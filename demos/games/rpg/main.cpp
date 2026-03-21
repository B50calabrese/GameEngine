#include <iostream>
#include <memory>
#include <random>
#include <string>
#include <vector>

#include <engine/core/application.h>
#include <engine/core/engine.h>
#include <engine/ecs/registry.h>
#include <engine/graphics/graphics_components.h>
#include <engine/graphics/renderer.h>
#include <engine/graphics/text_renderer.h>
#include <engine/input/action_manager.h>
#include <engine/input/input_manager.h>
#include <engine/scene/scene.h>
#include <engine/scene/scene_manager.h>
#include <engine/ui/ui_components.h>
#include <engine/ui/ui_systems.h>
#include <engine/util/tween_manager.h>

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

// --- Helper Functions ---
void DrawLegend(float x, float y) {
  float size = 20.0f;
  float spacing = 25.0f;

  engine::graphics::Renderer::Get().DrawQuad({x, y}, {size, size},
                                             {0.0f, 1.0f, 0.0f, 1.0f});
  engine::graphics::Renderer::Get().DrawText(
      "default", "Player", {x + size + 5, y}, 0.0f, 0.5f, {1, 1, 1, 1});

  engine::graphics::Renderer::Get().DrawQuad({x, y - spacing}, {size, size},
                                             {0.3f, 0.3f, 0.3f, 1.0f});
  engine::graphics::Renderer::Get().DrawText("default", "Floor",
                                             {x + size + 5, y - spacing}, 0.0f,
                                             0.5f, {1, 1, 1, 1});

  engine::graphics::Renderer::Get().DrawQuad({x, y - 2 * spacing}, {size, size},
                                             {0.1f, 0.1f, 0.1f, 1.0f});
  engine::graphics::Renderer::Get().DrawText("default", "Wall",
                                             {x + size + 5, y - 2 * spacing},
                                             0.0f, 0.5f, {1, 1, 1, 1});

  engine::graphics::Renderer::Get().DrawQuad({x, y - 3 * spacing}, {size, size},
                                             {1.0f, 0.8f, 0.0f, 1.0f});
  engine::graphics::Renderer::Get().DrawText("default", "Chest",
                                             {x + size + 5, y - 3 * spacing},
                                             0.0f, 0.5f, {1, 1, 1, 1});

  engine::graphics::Renderer::Get().DrawQuad({x, y - 4 * spacing}, {size, size},
                                             {0.0f, 0.5f, 1.0f, 1.0f});
  engine::graphics::Renderer::Get().DrawText("default", "Stairs",
                                             {x + size + 5, y - 4 * spacing},
                                             0.0f, 0.5f, {1, 1, 1, 1});
}

class MapScene : public engine::Scene {
 public:
  MapScene(const std::string& name) : engine::Scene(name) {}

  void OnAttach() override;
  void OnUpdate(float dt) override;
  void OnRender() override;
  void GenerateLevel();
  void HandleTile(int x, int y, TileType type);

 private:
  MapGenerator::MapData map_;
  glm::ivec2 player_pos_;
};

class VictoryOverlay : public engine::Scene {
 public:
  VictoryOverlay() : engine::Scene("VictoryOverlay") {}
  void OnAttach() override {
    engine::graphics::TextRenderer::Get().Init();
    engine::graphics::TextRenderer::Get().LoadFont("default", "arial.ttf", 24);
  }
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
    enemy_stats_ = {0, 0, 0, 0};
    enemy_stats_.max_hp = 5 + GameState::Get().current_floor * 2;
    enemy_stats_.hp = enemy_stats_.max_hp;
    enemy_stats_.atk = GameState::Get().current_floor;
    enemy_stats_.def = GameState::Get().current_floor / 2;
    flee_difficulty_ = 0.3f + (GameState::Get().current_floor * 0.05f);
  }

  void OnAttach() override {
    engine::graphics::TextRenderer::Get().Init();
    engine::graphics::TextRenderer::Get().LoadFont("default", "arial.ttf", 24);
    RefreshSelection();
  }

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
    engine::graphics::Renderer::Get().DrawQuad({0.0f, 0.0f}, {800.0f, 600.0f},
                                               {0.2f, 0.1f, 0.1f, 1.0f});

    // Enemy
    engine::graphics::Renderer::Get().DrawQuad({500, 300}, {100, 100},
                                               {1, 0, 0, 1});
    engine::graphics::Renderer::Get().DrawText(
        "default",
        "Enemy HP: " + std::to_string(enemy_stats_.hp) + "/" +
            std::to_string(enemy_stats_.max_hp),
        {500, 420}, 0.0f, 0.8f, {1, 1, 1, 1});

    // Player
    engine::graphics::Renderer::Get().DrawQuad({150, 150}, {80, 80},
                                               {0, 1, 0, 1});
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
};

class LevelUpOverlay : public engine::Scene {
 public:
  LevelUpOverlay() : engine::Scene("LevelUpOverlay") {
    pending_attacks_ = GameState::Get().GetRandomAttacks(3);
    options_count_ = 3;  // Initially ATK, DEF, HP
  }

  void OnAttach() override {
    engine::graphics::TextRenderer::Get().Init();
    engine::graphics::TextRenderer::Get().LoadFont("default", "arial.ttf", 24);
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
void MapScene::OnAttach() { GenerateLevel(); }

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
      glm::vec4 color;
      switch (map_.tiles[y * MAP_WIDTH + x]) {
        case TileType::Floor:
          color = {0.3f, 0.3f, 0.3f, 1.0f};
          break;
        case TileType::Wall:
          color = {0.1f, 0.1f, 0.1f, 1.0f};
          break;
        case TileType::Chest:
          color = {1.0f, 0.8f, 0.0f, 1.0f};
          break;
        case TileType::Stairs:
          color = {0.0f, 0.5f, 1.0f, 1.0f};
          break;
      }
      engine::graphics::Renderer::Get().DrawQuad(
          pos, {TILE_SIZE - 2, TILE_SIZE - 2}, color);
    }
  }

  // Draw Player
  engine::graphics::Renderer::Get().DrawQuad(
      {player_pos_.x * TILE_SIZE, player_pos_.y * TILE_SIZE},
      {TILE_SIZE - 2, TILE_SIZE - 2}, {0.0f, 1.0f, 0.0f, 1.0f});

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

class RpgApp : public engine::Application {
 public:
  void OnInit() override {
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
  void OnShutdown() override {}
  void OnUpdate(double dt) override {
    engine::util::TweenManager::Get().Update(static_cast<float>(dt));
  }
};

int main(void) {
  engine::EngineConfig config;
  config.window_width = 800;
  config.window_height = 600;
  config.asset_path = ENGINE_ASSETS_PATH;
  engine::Engine::Init(config);
  RpgApp app;
  app.Run();
  return 0;
}
