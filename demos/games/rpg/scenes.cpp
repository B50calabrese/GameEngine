#include "scenes.h"

#include <engine/core/engine.h>
#include <engine/graphics/renderer.h>
#include <engine/input/input_manager.h>
#include <engine/scene/scene_manager.h>

#include "../../common/menu_scene.h"

namespace rpg {

const float kTileSize = 40.0f;
const int kMapWidth = 20;
const int kMapHeight = 15;

MapScene::MapScene(const std::string& name) : engine::Scene(name) {}

void MapScene::OnAttach() { GenerateLevel(); }

void MapScene::GenerateLevel() {
  map_ = MapGenerator::Generate(kMapWidth, kMapHeight, std::random_device{}());
  player_pos_ = {map_.start_x, map_.start_y};
  GameState::Get().RefreshAttackUses();
}

void MapScene::OnUpdate(float dt) {
  int dx = 0, dy = 0;
  if (engine::InputManager::Get().IsKeyPressed(engine::KeyCode::kW))
    dy = 1;
  else if (engine::InputManager::Get().IsKeyPressed(engine::KeyCode::kS))
    dy = -1;
  else if (engine::InputManager::Get().IsKeyPressed(engine::KeyCode::kA))
    dx = -1;
  else if (engine::InputManager::Get().IsKeyPressed(engine::KeyCode::kD))
    dx = 1;

  if (dx != 0 || dy != 0) {
    int nx = player_pos_.x + dx;
    int ny = player_pos_.y + dy;

    if (nx >= 0 && nx < kMapWidth && ny >= 0 && ny < kMapHeight) {
      TileType target = map_.tiles[ny * kMapWidth + nx];
      if (target != TileType::kWall) {
        player_pos_ = {nx, ny};
        HandleTile(nx, ny, target);
      }
    }
  }
}

void MapScene::HandleTile(int x, int y, TileType type) {
  switch (type) {
    case TileType::kChest:
      map_.tiles[y * kMapWidth + x] = TileType::kFloor;
      engine::SceneManager::Get().PushScene(std::make_unique<LevelUpOverlay>());
      break;
    case TileType::kStairs:
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
  for (int y = 0; y < kMapHeight; ++y) {
    for (int x = 0; x < kMapWidth; ++x) {
      glm::vec2 pos = {x * kTileSize, y * kTileSize};
      glm::vec4 color;
      switch (map_.tiles[y * kMapWidth + x]) {
        case TileType::kFloor:
          color = {0.3f, 0.3f, 0.3f, 1.0f};
          break;
        case TileType::kWall:
          color = {0.1f, 0.1f, 0.1f, 1.0f};
          break;
        case TileType::kChest:
          color = {1.0f, 0.8f, 0.0f, 1.0f};
          break;
        case TileType::kStairs:
          color = {0.0f, 0.5f, 1.0f, 1.0f};
          break;
      }
      engine::graphics::Renderer::Get().DrawQuad(
          pos, {kTileSize - 2, kTileSize - 2}, color);
    }
  }

  engine::graphics::Renderer::Get().DrawQuad(
      {player_pos_.x * kTileSize, player_pos_.y * kTileSize},
      {kTileSize - 2, kTileSize - 2}, {0.0f, 1.0f, 0.0f, 1.0f});

  engine::graphics::Renderer::Get().DrawQuad(
      "default", "Floor: " + std::to_string(GameState::Get().current_floor),
      {10, 570}, 0.0f, 0.8f, {1, 1, 1, 1});
  engine::graphics::Renderer::Get().DrawQuad(
      "default",
      "HP: " + std::to_string(GameState::Get().player_stats.hp) + "/" +
          std::to_string(GameState::Get().player_stats.max_hp),
      {150, 570}, 0.0f, 0.8f, {1, 1, 1, 1});
  engine::graphics::Renderer::Get().DrawQuad(
      "default",
      "ATK: " + std::to_string(GameState::Get().player_stats.atk) +
          " DEF: " + std::to_string(GameState::Get().player_stats.def),
      {350, 570}, 0.0f, 0.8f, {1, 1, 1, 1});

  engine::graphics::Renderer::Get().Flush();
}

BattleScene::BattleScene(const std::string& name) : engine::Scene(name) {
  enemy_stats_ = {0, 0, 0, 0};
  enemy_stats_.max_hp = 5 + GameState::Get().current_floor * 2;
  enemy_stats_.hp = enemy_stats_.max_hp;
  enemy_stats_.atk = GameState::Get().current_floor;
  enemy_stats_.def = GameState::Get().current_floor / 2;
  flee_difficulty_ = 0.3f + (GameState::Get().current_floor * 0.05f);
}

void BattleScene::OnAttach() { RefreshSelection(); }

void BattleScene::RefreshSelection() {
  options_count_ = (int)GameState::Get().player_attacks.size() + 1;
}

void BattleScene::OnUpdate(float dt) {
  if (engine::InputManager::Get().IsKeyPressed(engine::KeyCode::kW)) {
    selected_index_ = (selected_index_ - 1 + options_count_) % options_count_;
  } else if (engine::InputManager::Get().IsKeyPressed(engine::KeyCode::kS)) {
    selected_index_ = (selected_index_ + 1) % options_count_;
  } else if (engine::InputManager::Get().IsKeyPressed(
                 engine::KeyCode::kSpace)) {
    ExecuteAction();
  }
}

void BattleScene::ExecuteAction() {
  if (selected_index_ < (int)GameState::Get().player_attacks.size()) {
    auto& attack = GameState::Get().player_attacks[selected_index_];
    if (attack.current_uses > 0) {
      attack.current_uses--;
      std::mt19937 gen(std::random_device{}());
      if (std::uniform_real_distribution<>(0.0, 1.0)(gen) < attack.hit_chance) {
        int damage = std::max(1, (int)(GameState::Get().player_stats.atk *
                                       attack.damage_multiplier) -
                                     enemy_stats_.def);
        enemy_stats_.hp -= damage;
        last_log_ = "You hit for " + std::to_string(damage) + "!";
      } else {
        last_log_ = "You missed!";
      }
      if (enemy_stats_.hp <= 0) {
        engine::SceneManager::Get().PopScene();
        return;
      }
      EnemyTurn();
    } else {
      last_log_ = "No uses left!";
    }
  } else {
    std::mt19937 gen(std::random_device{}());
    if (std::uniform_real_distribution<>(0.0, 1.0)(gen) > flee_difficulty_) {
      engine::SceneManager::Get().PopScene();
    } else {
      last_log_ = "Failed to run!";
      EnemyTurn();
    }
  }
}

void BattleScene::EnemyTurn() {
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

void BattleScene::OnRender() {
  engine::graphics::Renderer::Get().DrawQuad({0.0f, 0.0f}, {800.0f, 600.0f},
                                             {0.2f, 0.1f, 0.1f, 1.0f});
  engine::graphics::Renderer::Get().DrawQuad({500, 300}, {100, 100},
                                             {1, 0, 0, 1});
  engine::graphics::Renderer::Get().DrawQuad(
      "default",
      "Enemy HP: " + std::to_string(enemy_stats_.hp) + "/" +
          std::to_string(enemy_stats_.max_hp),
      {500, 420}, 0.0f, 0.8f, {1, 1, 1, 1});
  engine::graphics::Renderer::Get().DrawQuad({150, 150}, {80, 80},
                                             {0, 1, 0, 1});
  engine::graphics::Renderer::Get().DrawQuad(
      "default",
      "Player HP: " + std::to_string(GameState::Get().player_stats.hp) + "/" +
          std::to_string(GameState::Get().player_stats.max_hp),
      {150, 250}, 0.0f, 0.8f, {1, 1, 1, 1});

  for (int i = 0; i < (int)GameState::Get().player_attacks.size(); ++i) {
    auto& attack = GameState::Get().player_attacks[i];
    glm::vec4 color =
        (selected_index_ == i) ? glm::vec4(1, 1, 0, 1) : glm::vec4(1, 1, 1, 1);
    std::string text = attack.name + " (" +
                       std::to_string(attack.current_uses) + "/" +
                       std::to_string(attack.max_uses) + ")";
    engine::graphics::Renderer::Get().DrawQuad(
        "default", text, {50, 500 - (float)i * 30}, 0.0f, 0.7f, color);
  }
  glm::vec4 run_color = (selected_index_ == options_count_ - 1)
                            ? glm::vec4(1, 1, 0, 1)
                            : glm::vec4(1, 1, 1, 1);
  engine::graphics::Renderer::Get().DrawQuad(
      "default", "Run",
      {50, 500 - (float)GameState::Get().player_attacks.size() * 30}, 0.0f,
      0.7f, run_color);
  engine::graphics::Renderer::Get().DrawQuad("default", last_log_, {200, 50},
                                             0.0f, 0.8f, {1, 1, 1, 1});
  engine::graphics::Renderer::Get().Flush();
}

VictoryOverlay::VictoryOverlay() : engine::Scene("VictoryOverlay") {}
void VictoryOverlay::OnUpdate(float dt) {
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
void VictoryOverlay::OnRender() {
  engine::graphics::Renderer::Get().DrawQuad({200.0f, 150.0f}, {400.0f, 300.0f},
                                             {0.1f, 0.1f, 0.1f, 0.95f});
  engine::graphics::Renderer::Get().DrawQuad("default", "VICTORY!", {320, 350},
                                             0.0f, 1.5f, {1, 1, 1, 1});
  engine::graphics::Renderer::Get().DrawQuad(
      "default", "You cleared all 10 floors!", {250, 300}, 0.0f, 0.8f,
      {1, 1, 1, 1});
  engine::graphics::Renderer::Get().DrawQuad("default", "Press SPACE to Return",
                                             {300, 250}, 0.0f, 0.8f,
                                             {0.7f, 0.7f, 0.7f, 1.0f});
  engine::graphics::Renderer::Get().Flush();
}

LevelUpOverlay::LevelUpOverlay() : engine::Scene("LevelUpOverlay") {
  pending_attacks_ = GameState::Get().GetRandomAttacks(3);
  options_count_ = 3;
}
void LevelUpOverlay::OnUpdate(float dt) {
  if (engine::InputManager::Get().IsKeyPressed(engine::KeyCode::kW))
    selected_index_ = (selected_index_ - 1 + options_count_) % options_count_;
  else if (engine::InputManager::Get().IsKeyPressed(engine::KeyCode::kS))
    selected_index_ = (selected_index_ + 1) % options_count_;
  else if (engine::InputManager::Get().IsKeyPressed(engine::KeyCode::kSpace))
    ExecuteSelection();
}
void LevelUpOverlay::ExecuteSelection() {
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
    options_count_ = 3;
    selected_index_ = 0;
  } else {
    GameState::Get().player_attacks.push_back(
        pending_attacks_[selected_index_]);
    engine::SceneManager::Get().PopScene();
  }
}
void LevelUpOverlay::OnRender() {
  engine::graphics::Renderer::Get().DrawQuad({150.0f, 100.0f}, {500.0f, 400.0f},
                                             {0.1f, 0.1f, 0.2f, 0.95f});
  if (!stat_picked_) {
    engine::graphics::Renderer::Get().DrawQuad(
        "default", "LEVEL UP! Step 1: Choose Stat Boost", {200, 460}, 0.0f,
        1.0f, {1, 1, 1, 1});
    std::vector<std::string> options = {"+1 ATK", "+1 DEF", "+5 Max HP"};
    for (int i = 0; i < 3; ++i) {
      glm::vec4 color = (selected_index_ == i) ? glm::vec4(1, 1, 0, 1)
                                               : glm::vec4(1, 1, 1, 1);
      engine::graphics::Renderer::Get().DrawQuad(
          "default", options[i], {250, 350 - (float)i * 40}, 0.0f, 0.8f, color);
    }
  } else {
    engine::graphics::Renderer::Get().DrawQuad(
        "default", "LEVEL UP! Step 2: Choose New Attack", {200, 460}, 0.0f,
        1.0f, {1, 1, 1, 1});
    for (int i = 0; i < 3; ++i) {
      glm::vec4 color = (selected_index_ == i) ? glm::vec4(1, 1, 0, 1)
                                               : glm::vec4(1, 1, 1, 1);
      std::string desc =
          pending_attacks_[i].name + " (Mult: " +
          std::to_string(pending_attacks_[i].damage_multiplier).substr(0, 3) +
          ")";
      engine::graphics::Renderer::Get().DrawQuad(
          "default", desc, {200, 350 - (float)i * 40}, 0.0f, 0.8f, color);
    }
  }
  engine::graphics::Renderer::Get().Flush();
}

}  // namespace rpg
