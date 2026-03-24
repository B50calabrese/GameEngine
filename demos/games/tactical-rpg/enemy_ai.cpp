#include "enemy_ai.h"

#include <algorithm>
#include <random>

#include <glm/glm.hpp>

#include "components.h"
#include "systems.h"

namespace tactical_rpg {

void EnemyAI::ProcessTurn(engine::ecs::Registry& registry,
                          engine::ecs::EntityID active) {
  auto& active_grid_pos = registry.GetComponent<GridPositionComponent>(active);
  auto& active_stats = registry.GetComponent<Stats>(active);

  engine::ecs::EntityID closest_player = engine::ecs::INVALID_ENTITY;
  int min_dist = 1000;

  auto view = registry.GetView<GridPositionComponent, IdentityComponent,
                               TurnStateComponent>();
  for (auto entity : view) {
    auto& id = registry.GetComponent<IdentityComponent>(entity);
    auto& turn_state = registry.GetComponent<TurnStateComponent>(entity);
    auto& pos = registry.GetComponent<GridPositionComponent>(entity);

    if (!id.is_enemy && !turn_state.is_downed) {
      int dist = std::abs(pos.pos.x - active_grid_pos.pos.x) +
                 std::abs(pos.pos.y - active_grid_pos.pos.y);
      if (dist < min_dist) {
        min_dist = dist;
        closest_player = entity;
      }
    }
  }

  if (registry.IsAlive(closest_player)) {
    auto& target_pos =
        registry.GetComponent<GridPositionComponent>(closest_player).pos;
    glm::ivec2 diff = target_pos - active_grid_pos.pos;
    int dist_x = std::abs(diff.x);
    int dist_y = std::abs(diff.y);

    int total_speed = active_stats.speed;

    // Move as close as possible to the player within speed limits
    int move_x = std::min(total_speed, dist_x);
    if (diff.x < 0) move_x = -move_x;
    active_grid_pos.pos.x += move_x;
    total_speed -= std::abs(move_x);

    int move_y = std::min(total_speed, dist_y);
    if (diff.y < 0) move_y = -move_y;
    active_grid_pos.pos.y += move_y;

    auto& actions = registry.GetComponent<ActionListComponent>(active).actions;
    if (!actions.empty()) {
      auto action_entity = actions[0];
      auto& action_data =
          registry.GetComponent<ActionDataComponent>(action_entity);
      int final_dist = std::abs(target_pos.x - active_grid_pos.pos.x) +
                       std::abs(target_pos.y - active_grid_pos.pos.y);
      if (final_dist <= action_data.range) {
        std::mt19937 gen(std::random_device{}());
        int attack_roll = std::uniform_int_distribution<int>(1, 20)(gen) + 2;
        auto& target_stats = registry.GetComponent<Stats>(closest_player);
        if (attack_roll >= target_stats.ac) {
          CombatSystem::ApplyEffect(registry, action_entity, closest_player);
        }
      }
    }
  }
}

}  // namespace tactical_rpg
