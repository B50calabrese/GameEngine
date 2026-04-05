#include <iostream>
#include <memory>
#include <random>
#include <string>
#include <vector>

#include <engine/ecs/components/light.h>
#include <engine/ecs/components/occluder.h>
#include <engine/ecs/components/quad.h>
#include <engine/ecs/components/sprite.h>
#include <engine/ecs/components/text.h>
#include <engine/ecs/registry.h>
#include <engine/graphics/renderer.h>
#include <engine/graphics/text_renderer.h>
#include <engine/graphics/texture.h>
#include <engine/input/action_manager.h>
#include <engine/input/input_manager.h>
#include <engine/scene/scene.h>
#include <engine/scene/scene_manager.h>
#include <engine/ui/ui_components.h>
#include <engine/ui/ui_systems.h>
#include <engine/util/tween_manager.h>

#include "../../common/demo_utils.h"
#include "../../common/menu_scene.h"
#include "game_state.h"
#include "map_generator.h"

// Forward declarations
class MapScene;
class BattleScene;
class LevelUpOverlay;

// --- Constants ---
const float TILE_SIZE = 40.0f;
const int MAP_WIDTH = 20;
const int MAP_HEIGHT = 15;

class MapScene : public engine::Scene {
 public:
  MapScene(const std::string& name) : engine::Scene(name) {}

  void OnAttach() override {
    tile_textures_[TileType::Floor] =
        engine::graphics::Texture::Load("textures/floor.png");
    tile_textures_[TileType::Chest] =
        engine::graphics::Texture::Load("textures/chest.png");
    tile_textures_[TileType::Stairs] =
        engine::graphics::Texture::Load("textures/stairs.png");
    player_tex_ = engine::graphics::Texture::Load("textures/knight_idle.png");
    GenerateLevel();
  }
  void OnUpdate(float dt) override;
  void OnRender() override;
  void GenerateLevel();
  void HandleTile(int x, int y, TileType type);

 private:
  MapGenerator::MapData map_;
  glm::ivec2 player_pos_;
  std::unordered_map<TileType, std::shared_ptr<engine::graphics::Texture>>
      tile_textures_;
  std::shared_ptr<engine::graphics::Texture> player_tex_;
};

class VictoryOverlay : public engine::Scene {
 public:
  VictoryOverlay() : engine::Scene("VictoryOverlay") {}
  void OnUpdate(float dt) override {
    if (engine::InputManager::Get().IsKeyPressed(engine::KeyCode::kSpace)) {
      engine::SceneManager::Get().SetScene(
          std::make_unique<demos::common::BaseMenuScene>(
              "RPG ROGUELIKE DEMO",
              std::vector<demos::common::BaseMenuScene::MenuItem>{
                  {"New Run",
                   []() {
                     GameState::Get().ResetRun();
                     engine::SceneManager::Get().SetScene(
                         std::make_unique<MapScene>("Map"));
                   }},
                  {"Quit", []() { engine::Engine::Shutdown(); }}}));
    }
  }
  void OnRender() override {
    engine::graphics::Renderer::Get().DrawQuad(
        {200.0f, 150.0f}, {400.0f, 300.0f}, {0.1f, 0.1f, 0.1f, 0.95f});
    engine::graphics::Renderer::Get().DrawText(
        "default", "VICTORY!", {320, 350}, 0.0f, 1.5f, {1, 1, 1, 1});
    engine::graphics::Renderer::Get().DrawText(
        "default", "You cleared all 10 floors!", {250, 300}, 0.0f, 0.8f,
        {1, 1, 1, 1});
    engine::graphics::Renderer::Get().DrawText(
        "default", "Press SPACE to Return", {300, 250}, 0.0f, 0.8f,
        {0.7f, 0.7f, 0.7f, 1.0f});
    engine::graphics::Renderer::Get().Flush();
  }
};

class BattleScene : public engine::Scene {
 public:
  BattleScene(const std::string& name) : engine::Scene(name) {
    bg_tex_ = engine::graphics::Texture::Load("textures/rpg_bg.png");
    enemy_tex_ = engine::graphics::Texture::Load("textures/robot_idle.png");
    player_tex_ = engine::graphics::Texture::Load("textures/knight_idle.png");

    enemy_stats_ = {0, 0, 0, 0};
    enemy_stats_.max_hp = 5 + GameState::Get().current_floor * 2;
    enemy_stats_.hp = enemy_stats_.max_hp;
    enemy_stats_.atk = GameState::Get().current_floor;
    enemy_stats_.def = GameState::Get().current_floor / 2;
    flee_difficulty_ = 0.3f + (GameState::Get().current_floor * 0.05f);
  }

  void OnAttach() override { RefreshSelection(); }

  void RefreshSelection() {
    options_count_ =
        (int)GameState::Get().player_attacks.size() + 1;  // Attacks + Run
  }

  void OnUpdate(float dt) override {
    if (engine::InputManager::Get().IsKeyPressed(engine::KeyCode::kW)) {
      selected_index_ = (selected_index_ - 1 + options_count_) % options_count_;
    } else if (engine::InputManager::Get().IsKeyPressed(engine::KeyCode::kS)) {
      selected_index_ = (selected_index_ + 1) % options_count_;
    } else if (engine::InputManager::Get().IsKeyPressed(
                   engine::KeyCode::kSpace)) {
      ExecuteAction();
    }
  }

  void ExecuteAction() {
    if (selected_index_ < (int)GameState::Get().player_attacks.size()) {
      // Attack
      auto& attack = GameState::Get().player_attacks[selected_index_];
      if (attack.current_uses > 0) {
        attack.current_uses--;
        std::mt19937 gen(std::random_device{}());
        if (std::uniform_real_distribution<>(0.0, 1.0)(gen) <
            attack.hit_chance) {
          int damage = std::max(1, (int)(GameState::Get().player_stats.atk *
                                         attack.damage_multiplier) -
                                       enemy_stats_.def);
          enemy_stats_.hp -= damage;
          last_log_ = "You hit for " + std::to_string(damage) + "!";
        } else {
          last_log_ = "You missed!";
        }
        if (enemy_stats_.hp <= 0) {
          engine::SceneManager::Get().PopScene();  // Return to map
          return;
        }
        EnemyTurn();
      } else {
        last_log_ = "No uses left!";
      }
    } else {
      // Run
      std::mt19937 gen(std::random_device{}());
      if (std::uniform_real_distribution<>(0.0, 1.0)(gen) > flee_difficulty_) {
        engine::SceneManager::Get().PopScene();
      } else {
        last_log_ = "Failed to run!";
        EnemyTurn();
      }
    }
  }

  void EnemyTurn() {
    std::mt19937 gen(std::random_device{}());
    int damage =
        std::max(1, enemy_stats_.atk - GameState::Get().player_stats.def);
    GameState::Get().player_stats.hp -= damage;
    last_log_ += " Enemy hits for " + std::to_string(damage) + "!";
    if (GameState::Get().player_stats.hp <= 0) {
      engine::SceneManager::Get().SetScene(
          std::make_unique<demos::common::BaseMenuScene>(
              "RPG ROGUELIKE DEMO",
              std::vector<demos::common::BaseMenuScene::MenuItem>{
                  {"New Run",
                   []() {
                     GameState::Get().ResetRun();
                     engine::SceneManager::Get().SetScene(
                         std::make_unique<MapScene>("Map"));
                   }},
                  {"Quit", []() { engine::Engine::Shutdown(); }}}));
    }
  }

  void OnRender() override {
    if (bg_tex_) {
      engine::graphics::Renderer::Get().DrawTexturedQuad(
          {400.0f, 300.0f}, {800.0f, 600.0f}, bg_tex_.get());
    } else {
      engine::graphics::Renderer::Get().DrawQuad({0.0f, 0.0f}, {800.0f, 600.0f},
                                                 {0.2f, 0.1f, 0.1f, 1.0f});
    }

    // Enemy
    if (enemy_tex_) {
      engine::graphics::Renderer::Get().DrawTexturedQuad({500, 300}, {128, 128},
                                                         enemy_tex_.get());
    }
    engine::graphics::Renderer::Get().DrawText(
        "default",
        "Enemy HP: " + std::to_string(enemy_stats_.hp) + "/" +
            std::to_string(enemy_stats_.max_hp),
        {500, 420}, 0.0f, 0.8f, {1, 1, 1, 1});

    // Player
    if (player_tex_) {
      engine::graphics::Renderer::Get().DrawTexturedQuad({150, 150}, {128, 128},
                                                         player_tex_.get());
    }
    engine::graphics::Renderer::Get().DrawText(
        "default",
        "Player HP: " + std::to_string(GameState::Get().player_stats.hp) + "/" +
            std::to_string(GameState::Get().player_stats.max_hp),
        {150, 250}, 0.0f, 0.8f, {1, 1, 1, 1});

    // Actions
    for (int i = 0; i < (int)GameState::Get().player_attacks.size(); ++i) {
      auto& attack = GameState::Get().player_attacks[i];
      glm::vec4 color = (selected_index_ == i) ? glm::vec4(1, 1, 0, 1)
                                               : glm::vec4(1, 1, 1, 1);
      std::string text = attack.name + " (" +
                         std::to_string(attack.current_uses) + "/" +
                         std::to_string(attack.max_uses) + ")";
      engine::graphics::Renderer::Get().DrawText(
          "default", text, {50, 500 - (float)i * 30}, 0.0f, 0.7f, color);
    }
    glm::vec4 run_color = (selected_index_ == options_count_ - 1)
                              ? glm::vec4(1, 1, 0, 1)
                              : glm::vec4(1, 1, 1, 1);
    engine::graphics::Renderer::Get().DrawText(
        "default", "Run",
        {50, 500 - (float)GameState::Get().player_attacks.size() * 30}, 0.0f,
        0.7f, run_color);

    engine::graphics::Renderer::Get().DrawText("default", last_log_, {200, 50},
                                               0.0f, 0.8f, {1, 1, 1, 1});

    engine::graphics::Renderer::Get().Flush();
  }

 private:
  Stats enemy_stats_;
  float flee_difficulty_;
  int selected_index_ = 0;
  int options_count_ = 0;
  std::string last_log_ = "Your Turn";

  std::shared_ptr<engine::graphics::Texture> bg_tex_;
  std::shared_ptr<engine::graphics::Texture> enemy_tex_;
  std::shared_ptr<engine::graphics::Texture> player_tex_;
};

class LevelUpOverlay : public engine::Scene {
 public:
  LevelUpOverlay() : engine::Scene("LevelUpOverlay") {
    pending_attacks_ = GameState::Get().GetRandomAttacks(3);
    options_count_ = 3;  // Initially ATK, DEF, HP
  }

  void OnUpdate(float dt) override {
    if (engine::InputManager::Get().IsKeyPressed(engine::KeyCode::kW)) {
      selected_index_ = (selected_index_ - 1 + options_count_) % options_count_;
    } else if (engine::InputManager::Get().IsKeyPressed(engine::KeyCode::kS)) {
      selected_index_ = (selected_index_ + 1) % options_count_;
    } else if (engine::InputManager::Get().IsKeyPressed(
                   engine::KeyCode::kSpace)) {
      ExecuteSelection();
    }
  }

  void ExecuteSelection() {
    if (!stat_picked_) {
      switch (selected_index_) {
        case 0:
          GameState::Get().player_stats.atk += 1;
          break;
        case 1:
          GameState::Get().player_stats.def += 1;
          break;
        case 2:
          GameState::Get().player_stats.max_hp += 5;
          GameState::Get().player_stats.hp += 5;
          break;
      }
      stat_picked_ = true;
      options_count_ = 3;  // Pick between 3 attacks
      selected_index_ = 0;
    } else {
      GameState::Get().player_attacks.push_back(
          pending_attacks_[selected_index_]);
      engine::SceneManager::Get().PopScene();
    }
  }

  void OnRender() override {
    engine::graphics::Renderer::Get().DrawQuad(
        {150.0f, 100.0f}, {500.0f, 400.0f}, {0.1f, 0.1f, 0.2f, 0.95f});

    if (!stat_picked_) {
      engine::graphics::Renderer::Get().DrawText(
          "default", "LEVEL UP! Step 1: Choose Stat Boost", {200, 460}, 0.0f,
          1.0f, {1, 1, 1, 1});
      std::vector<std::string> options = {"+1 ATK", "+1 DEF", "+5 Max HP"};
      for (int i = 0; i < 3; ++i) {
        glm::vec4 color = (selected_index_ == i) ? glm::vec4(1, 1, 0, 1)
                                                 : glm::vec4(1, 1, 1, 1);
        engine::graphics::Renderer::Get().DrawText("default", options[i],
                                                   {250, 350 - (float)i * 40},
                                                   0.0f, 0.8f, color);
      }
    } else {
      engine::graphics::Renderer::Get().DrawText(
          "default", "LEVEL UP! Step 2: Choose New Attack", {200, 460}, 0.0f,
          1.0f, {1, 1, 1, 1});
      for (int i = 0; i < 3; ++i) {
        glm::vec4 color = (selected_index_ == i) ? glm::vec4(1, 1, 0, 1)
                                                 : glm::vec4(1, 1, 1, 1);
        std::string desc =
            pending_attacks_[i].name + " (Mult: " +
            std::to_string(pending_attacks_[i].damage_multiplier).substr(0, 3) +
            ")";
        engine::graphics::Renderer::Get().DrawText(
            "default", desc, {200, 350 - (float)i * 40}, 0.0f, 0.8f, color);
      }
    }
    engine::graphics::Renderer::Get().Flush();
  }

 private:
  bool stat_picked_ = false;
  int selected_index_ = 0;
  int options_count_ = 0;
  std::vector<Attack> pending_attacks_;
};

// --- Map engine::Scene Implementation ---
void MapScene::GenerateLevel() {
  map_ = MapGenerator::Generate(MAP_WIDTH, MAP_HEIGHT, std::random_device{}());
  player_pos_ = {map_.start_x, map_.start_y};
  GameState::Get().RefreshAttackUses();
}

void MapScene::OnUpdate(float dt) {
  int dx = 0, dy = 0;
  if (engine::InputManager::Get().IsKeyPressed(engine::KeyCode::kW)) {
    dy = 1;
  } else if (engine::InputManager::Get().IsKeyPressed(engine::KeyCode::kS)) {
    dy = -1;
  } else if (engine::InputManager::Get().IsKeyPressed(engine::KeyCode::kA)) {
    dx = -1;
  } else if (engine::InputManager::Get().IsKeyPressed(engine::KeyCode::kD)) {
    dx = 1;
  }

  if (dx != 0 || dy != 0) {
    int nx = player_pos_.x + dx;
    int ny = player_pos_.y + dy;

    if (nx >= 0 && nx < MAP_WIDTH && ny >= 0 && ny < MAP_HEIGHT) {
      TileType target = map_.tiles[ny * MAP_WIDTH + nx];
      if (target != TileType::Wall) {
        player_pos_ = {nx, ny};
        HandleTile(nx, ny, target);
      }
    }
  }
}

void MapScene::HandleTile(int x, int y, TileType type) {
  switch (type) {
    case TileType::Chest:
      map_.tiles[y * MAP_WIDTH + x] = TileType::Floor;
      engine::SceneManager::Get().PushScene(std::make_unique<LevelUpOverlay>());
      break;
    case TileType::Stairs:
      if (GameState::Get().current_floor >= 10) {
        engine::SceneManager::Get().PushScene(
            std::make_unique<VictoryOverlay>());
      } else {
        GameState::Get().current_floor++;
        GenerateLevel();
      }
      break;
    default: {
      float chance =
          0.05f + (GameState::Get().current_floor - 1) * (0.15f / 9.0f);
      std::random_device rd;
      std::mt19937 gen(rd());
      if (std::uniform_real_distribution<>(0.0, 1.0)(gen) < chance) {
        engine::SceneManager::Get().PushScene(
            std::make_unique<BattleScene>("Battle"));
      }
    } break;
  }
}

void MapScene::OnRender() {
  for (int y = 0; y < MAP_HEIGHT; ++y) {
    for (int x = 0; x < MAP_WIDTH; ++x) {
      glm::vec2 pos = {x * TILE_SIZE, y * TILE_SIZE};
      TileType type = map_.tiles[y * MAP_WIDTH + x];
      if (type == TileType::Wall) {
        engine::graphics::Renderer::Get().DrawQuad(
            pos + glm::vec2(TILE_SIZE / 2), {TILE_SIZE, TILE_SIZE},
            {0.3f, 0.3f, 0.3f, 1.0f});
      } else if (tile_textures_.count(type) && tile_textures_[type]) {
        engine::graphics::Renderer::Get().DrawTexturedQuad(
            pos + glm::vec2(TILE_SIZE / 2), {TILE_SIZE, TILE_SIZE},
            tile_textures_[type].get());
      }
    }
  }

  // Draw Player
  if (player_tex_) {
    engine::graphics::Renderer::Get().DrawTexturedQuad(
        {player_pos_.x * TILE_SIZE + TILE_SIZE / 2,
         player_pos_.y * TILE_SIZE + TILE_SIZE / 2},
        {TILE_SIZE * 1.5f, TILE_SIZE * 1.5f}, player_tex_.get());
  }

  // Draw HUD
  engine::graphics::Renderer::Get().DrawText(
      "default", "Floor: " + std::to_string(GameState::Get().current_floor),
      {10, 570}, 0.0f, 0.8f, {1, 1, 1, 1});
  engine::graphics::Renderer::Get().DrawText(
      "default",
      "HP: " + std::to_string(GameState::Get().player_stats.hp) + "/" +
          std::to_string(GameState::Get().player_stats.max_hp),
      {150, 570}, 0.0f, 0.8f, {1, 1, 1, 1});
  engine::graphics::Renderer::Get().DrawText(
      "default",
      "ATK: " + std::to_string(GameState::Get().player_stats.atk) +
          " DEF: " + std::to_string(GameState::Get().player_stats.def),
      {350, 570}, 0.0f, 0.8f, {1, 1, 1, 1});

  engine::graphics::Renderer::Get().Flush();
}

class RpgApp : public demos::common::BaseDemoApp {
 public:
  void OnDemoInit() override {
    std::vector<demos::common::BaseMenuScene::MenuItem> items = {
        {"New Run",
         []() {
           GameState::Get().ResetRun();
           engine::SceneManager::Get().SetScene(
               std::make_unique<MapScene>("Map"));
         }},
        {"Quit", []() { engine::Engine::Shutdown(); }}};
    engine::SceneManager::Get().SetScene(
        std::make_unique<demos::common::BaseMenuScene>("RPG ROGUELIKE DEMO",
                                                       items));
  }
};

int main(void) {
  engine::EngineConfig config;
  config.window_width = 800;
  config.window_height = 600;
  config.asset_path = ENGINE_ASSETS_PATH;
  return demos::common::DemoRunner::Run<RpgApp>(config);
}
