#include "battle_scene.h"

#include <algorithm>
#include <random>

#include <engine/core/engine.h>
#include <engine/ecs/components/transform.h>
#include <engine/graphics/renderer.h>
#include <engine/input/input_manager.h>
#include <engine/scene/scene_manager.h>
#include <engine/util/logger.h>

#include "action_registry.h"
#include "character_renderer.h"
#include "class_registry.h"
#include "combat_rules.h"
#include "components.h"
#include "enemy_ai.h"
#include "game_manager.h"
#include "grid_renderer.h"
#include "systems.h"

namespace tactical_rpg {

void BattleScene::OnAttach() {
  ActionRegistry::Get().ClearCache();
  BattleGrid::Setup(GetRegistry());
  SetupEnemies();
  turn_manager_.RollInitiative(GetRegistry());
  turn_manager_.NextTurn(GetRegistry());

  auto active = turn_manager_.GetActiveCharacter();
  if (GetRegistry().IsAlive(active)) {
    cursor_pos_ = GetRegistry().GetComponent<GridPositionComponent>(active).pos;
  }
}

void BattleScene::SetupEnemies() {
  std::mt19937 gen(std::random_device{}());
  std::uniform_int_distribution<int> class_dist(0, 11);

  enemy_entities_.clear();
  int num_enemies = 2 + (difficulty_ / 2);
  for (int i = 0; i < num_enemies; ++i) {
    ClassType type = static_cast<ClassType>(class_dist(gen));
    auto entity = ClassRegistry::Get().CreateCharacter(
        GetRegistry(), type, "Enemy " + std::to_string(i + 1), true);
    GetRegistry().AddComponent(
        entity, GridPositionComponent{{i * 2, BattleGrid::kSize - 1}});
    enemy_entities_.push_back(entity);
  }

  party_entities_.clear();
  for (int i = 0; i < (int)party_data_.size(); ++i) {
    auto entity = ClassRegistry::Get().CreateCharacter(
        GetRegistry(), party_data_[i].class_type, party_data_[i].name, false);
    GetRegistry().AddComponent(entity, GridPositionComponent{{i * 2, 0}});
    party_entities_.push_back(entity);
  }
}

void BattleScene::OnUpdate(float dt) {
  SyncTransformSystem::Update(GetRegistry(), tile_visual_size_, grid_offset_);

  for (auto it = floating_texts_.begin(); it != floating_texts_.end();) {
    it->lifetime -= dt;
    it->position.y += 20.0f * dt;
    if (it->lifetime <= 0) {
      it = floating_texts_.erase(it);
    } else {
      ++it;
    }
  }

  if (is_battle_over_) {
    if (engine::InputManager::Get().IsKeyPressed(engine::KeyCode::kSpace)) {
      auto view =
          GetRegistry().GetView<IdentityComponent, TurnStateComponent>();
      bool any_enemy_alive = false;
      for (auto entity : view) {
        if (GetRegistry().GetComponent<IdentityComponent>(entity).is_enemy &&
            !GetRegistry().GetComponent<TurnStateComponent>(entity).is_downed) {
          any_enemy_alive = true;
          break;
        }
      }
      GameManager::Get().CompleteBattle(!any_enemy_alive);
    }
    return;
  }

  auto active = turn_manager_.GetActiveCharacter();
  if (!GetRegistry().IsAlive(active)) return;

  if (!GetRegistry().GetComponent<IdentityComponent>(active).is_enemy) {
    HandlePlayerTurn(dt);
  } else {
    HandleEnemyAI();
  }

  if (turn_manager_.IsBattleOver(GetRegistry())) {
    is_battle_over_ = true;
    auto view = GetRegistry().GetView<IdentityComponent, TurnStateComponent>();
    bool any_enemy_alive = false;
    for (auto entity : view) {
      if (GetRegistry().GetComponent<IdentityComponent>(entity).is_enemy &&
          !GetRegistry().GetComponent<TurnStateComponent>(entity).is_downed) {
        any_enemy_alive = true;
        break;
      }
    }
    last_log_ =
        !any_enemy_alive ? "VICTORY! Press SPACE" : "DEFEAT! Press SPACE";
  }
}

void BattleScene::HandlePlayerTurn(float dt) {
  auto& input = engine::InputManager::Get();
  auto active = turn_manager_.GetActiveCharacter();
  auto& active_grid_pos =
      GetRegistry().GetComponent<GridPositionComponent>(active);
  auto& active_turn_state =
      GetRegistry().GetComponent<TurnStateComponent>(active);
  auto& active_stats = GetRegistry().GetComponent<Stats>(active);
  auto& active_id = GetRegistry().GetComponent<IdentityComponent>(active);

  // Movement
  if (input.IsKeyPressed(engine::KeyCode::kW))
    cursor_pos_.y = std::max(0, cursor_pos_.y - 1);
  if (input.IsKeyPressed(engine::KeyCode::kS))
    cursor_pos_.y = std::min(BattleGrid::kSize - 1, cursor_pos_.y + 1);
  if (input.IsKeyPressed(engine::KeyCode::kA))
    cursor_pos_.x = std::max(0, cursor_pos_.x - 1);
  if (input.IsKeyPressed(engine::KeyCode::kD))
    cursor_pos_.x = std::min(BattleGrid::kSize - 1, cursor_pos_.x + 1);

  // Selection
  if (input.IsKeyPressed(engine::KeyCode::k1)) selected_action_index_ = -1;
  if (input.IsKeyPressed(engine::KeyCode::k2)) selected_action_index_ = 0;
  if (input.IsKeyPressed(engine::KeyCode::k3)) selected_action_index_ = 1;

  if (input.IsKeyPressed(engine::KeyCode::kSpace)) {
    if (selected_action_index_ == -1) {
      if (CombatRules::CanMoveTo(GetRegistry(), active, cursor_pos_.x,
                                 cursor_pos_.y)) {
        int dist = std::abs(cursor_pos_.x - active_grid_pos.pos.x) +
                   std::abs(cursor_pos_.y - active_grid_pos.pos.y);
        active_grid_pos.pos = cursor_pos_;
        active_turn_state.movement_remaining -= dist;

        if (BattleGrid::GetTerrain(GetRegistry(), cursor_pos_.x,
                                   cursor_pos_.y) == TerrainType::Damage) {
          active_stats.current_hp -= 2;
          last_log_ = active_id.name + " took damage!";
          auto& trans =
              GetRegistry().GetComponent<engine::ecs::components::Transform>(
                  active);
          AddFloatingText("-2", trans.position, {1, 0, 0, 1});
        }
      }
    } else {
      auto& actions =
          GetRegistry().GetComponent<ActionListComponent>(active).actions;
      if (selected_action_index_ < (int)actions.size()) {
        auto action_entity = actions[selected_action_index_];
        auto& action_data =
            GetRegistry().GetComponent<ActionDataComponent>(action_entity);
        bool can_use =
            (action_data.is_bonus_action ? !active_turn_state.bonus_action_used
                                         : !active_turn_state.action_used);

        if (can_use) {
          // Find target
          engine::ecs::EntityID target = engine::ecs::kInvalidEntity;
          auto view = GetRegistry()
                          .GetView<GridPositionComponent, TurnStateComponent>();
          for (auto entity : view) {
            if (!GetRegistry()
                     .GetComponent<TurnStateComponent>(entity)
                     .is_downed &&
                GetRegistry().GetComponent<GridPositionComponent>(entity).pos ==
                    cursor_pos_) {
              target = entity;
              break;
            }
          }

          if (GetRegistry().IsAlive(target) &&
              CombatRules::IsInRange(GetRegistry(), active, target,
                                     action_data.range)) {
            std::mt19937 gen(std::random_device{}());
            int roll = std::uniform_int_distribution<int>(1, 20)(gen);

            auto& target_stats = GetRegistry().GetComponent<Stats>(target);
            auto& target_id =
                GetRegistry().GetComponent<IdentityComponent>(target);

            // For now, always check against AC for non-heal actions
            bool is_heal = (action_data.type == ActionType::Ability &&
                            action_data.name == "Second Wind") ||
                           (action_data.name == "Lay on Hands");

            if (is_heal || roll + 5 >= target_stats.ac) {
              last_log_ = active_id.name + " used " + action_data.name;
              int result = CombatSystem::ApplyEffect(GetRegistry(),
                                                     action_entity, target);
              auto& trans =
                  GetRegistry()
                      .GetComponent<engine::ecs::components::Transform>(target);
              if (result < 0) {
                AddFloatingText(std::to_string(result), trans.position,
                                {1, 0, 0, 1});
              } else if (result > 0) {
                AddFloatingText("+" + std::to_string(result), trans.position,
                                {0, 1, 0, 1});
              }

              if (target_stats.current_hp <= 0) {
                last_log_ += ". " + target_id.name + " is down!";
              }
            } else {
              last_log_ = active_id.name + " missed " + target_id.name + "!";
            }

            if (action_data.is_bonus_action)
              active_turn_state.bonus_action_used = true;
            else
              active_turn_state.action_used = true;
          }
        }
      }
    }
  }

  if (input.IsKeyPressed(engine::KeyCode::kEnter)) {
    turn_manager_.NextTurn(GetRegistry());
    auto next = turn_manager_.GetActiveCharacter();
    if (GetRegistry().IsAlive(next)) {
      cursor_pos_ = GetRegistry().GetComponent<GridPositionComponent>(next).pos;
      last_log_ =
          GetRegistry().GetComponent<IdentityComponent>(next).name + "'s Turn!";
    }
  }
}

void BattleScene::HandleEnemyAI() {
  auto active = turn_manager_.GetActiveCharacter();
  EnemyAI::ProcessTurn(
      GetRegistry(), active,
      [this](const std::string& text, glm::vec2 pos, glm::vec4 color) {
        AddFloatingText(text, pos, color);
      });

  // Auto end turn
  turn_manager_.NextTurn(GetRegistry());
  auto next = turn_manager_.GetActiveCharacter();
  if (GetRegistry().IsAlive(next)) {
    cursor_pos_ = GetRegistry().GetComponent<GridPositionComponent>(next).pos;
    last_log_ =
        GetRegistry().GetComponent<IdentityComponent>(next).name + "'s Turn!";
  }
}

void BattleScene::OnRender() {
  GridRenderer::Render(GetRegistry(), grid_offset_, tile_visual_size_,
                       cursor_pos_);
  CharacterRenderer::Render(GetRegistry(), turn_manager_.GetActiveCharacter(),
                            grid_offset_, tile_visual_size_);

  for (const auto& ft : floating_texts_) {
    engine::graphics::utils::RenderQueue::Default().Submit(
        "default", ft.text, ft.position, 0.0f, 0.5f, ft.color);
  }

  RenderUI();
}

void BattleScene::RenderUI() {
  engine::graphics::utils::RenderQueue::Default().Submit(
      "default", last_log_, {100, 650}, 0.0f, 1.0f, {1, 1, 1, 1});

  // Tile Tooltip
  TerrainType terrain =
      BattleGrid::GetTerrain(GetRegistry(), cursor_pos_.x, cursor_pos_.y);
  std::string terrain_name = "Unknown";
  std::string terrain_desc = "";
  switch (terrain) {
    case TerrainType::Normal:
      terrain_name = "Grass";
      terrain_desc = "Normal terrain.";
      break;
    case TerrainType::Slow:
      terrain_name = "Thicket";
      terrain_desc = "Slows movement.";
      break;
    case TerrainType::Damage:
      terrain_name = "Lava";
      terrain_desc = "Deals damage on enter.";
      break;
    case TerrainType::Impassible:
      terrain_name = "Wall";
      terrain_desc = "Cannot pass.";
      break;
  }
  engine::graphics::utils::RenderQueue::Default().Submit(
      "default", "Tile: " + terrain_name, {100, 600}, 0.0f, 0.6f,
      {0.7f, 0.7f, 1.0f, 1.0f});
  engine::graphics::utils::RenderQueue::Default().Submit(
      "default", terrain_desc, {100, 580}, 0.0f, 0.4f,
      {0.6f, 0.6f, 0.9f, 1.0f});

  auto active = turn_manager_.GetActiveCharacter();
  if (GetRegistry().IsAlive(active) &&
      !GetRegistry().GetComponent<IdentityComponent>(active).is_enemy) {
    auto& turn_state = GetRegistry().GetComponent<TurnStateComponent>(active);
    auto& stats = GetRegistry().GetComponent<Stats>(active);
    auto& id = GetRegistry().GetComponent<IdentityComponent>(active);
    auto& actions =
        GetRegistry().GetComponent<ActionListComponent>(active).actions;

    std::string move_info =
        "Movement: " + std::to_string(turn_state.movement_remaining) + "/" +
        std::to_string(stats.speed);
    engine::graphics::utils::RenderQueue::Default().Submit(
        "default", move_info, {700, 500}, 0.0f, 0.7f, {1, 1, 1, 1});

    engine::graphics::utils::RenderQueue::Default().Submit(
        "default", "1: Move Mode", {700, 450}, 0.0f, 0.7f,
        (selected_action_index_ == -1 ? glm::vec4(1, 1, 0, 1)
                                      : glm::vec4(1, 1, 1, 1)));

    for (int i = 0; i < (int)actions.size(); ++i) {
      auto action_entity = actions[i];
      auto& action_data =
          GetRegistry().GetComponent<ActionDataComponent>(action_entity);
      bool is_selected = (selected_action_index_ == i);
      glm::vec4 color =
          (is_selected ? glm::vec4(1, 1, 0, 1) : glm::vec4(1, 1, 1, 1));
      engine::graphics::utils::RenderQueue::Default().Submit(
          "default", std::to_string(i + 2) + ": " + action_data.name,
          {700, 400 - i * 40.0f}, 0.0f, 0.7f, color);

      if (is_selected) {
        std::string desc = action_data.description;
        auto& effects =
            GetRegistry().GetComponent<EffectsListComponent>(action_entity);
        for (auto effect : effects.effects) {
          if (GetRegistry().HasComponent<DamageEffectComponent>(effect)) {
            auto& dmg =
                GetRegistry().GetComponent<DamageEffectComponent>(effect);
            desc += " (Damage: " + std::to_string(dmg.num_dice) + "d" +
                    std::to_string(dmg.dice_size) + "+" +
                    std::to_string(dmg.modifier) + ")";
          } else if (GetRegistry().HasComponent<HealEffectComponent>(effect)) {
            auto& heal =
                GetRegistry().GetComponent<HealEffectComponent>(effect);
            desc += " (Heal: " + std::to_string(heal.num_dice) + "d" +
                    std::to_string(heal.dice_size) + "+" +
                    std::to_string(heal.modifier) + ")";
          }
        }
        engine::graphics::utils::RenderQueue::Default().Submit(
            "default", desc, {700, 370 - i * 40.0f}, 0.0f, 0.5f,
            {0.8f, 0.8f, 0.8f, 1.0f});
      }
    }

    engine::graphics::utils::RenderQueue::Default().Submit(
        "default", "ENTER: End Turn", {700, 100}, 0.0f, 0.7f, {1, 1, 1, 1});
    engine::graphics::utils::RenderQueue::Default().Submit(
        "default",
        id.name + " HP: " + std::to_string(stats.current_hp) + "/" +
            std::to_string(stats.max_hp),
        {100, 50}, 0.0f, 0.7f, {1, 1, 1, 1});
  }

  engine::graphics::Renderer::Get().Flush();
}

void BattleScene::AddFloatingText(const std::string& text, glm::vec2 pos,
                                  glm::vec4 color) {
  floating_texts_.push_back({text, pos, 1.0f, color});
}

}  // namespace tactical_rpg
