#ifndef DEMOS_GAMES_PLATFORMER_COMPONENTS_H_
#define DEMOS_GAMES_PLATFORMER_COMPONENTS_H_

#include <glm/vec2.hpp>

namespace platformer {

struct PlayerComponent {
  bool is_grounded = false;
  float jump_force = 600.0f;
  float move_speed = 300.0f;
};

enum class PlatformType { Stationary, Moving, Temporary };

struct PlatformComponent {
  PlatformType type = PlatformType::Stationary;
  glm::vec2 start_pos;
  glm::vec2 end_pos;
  float timer = 0.0f;
  bool touched = false;
  bool active = true;
};

struct EnemyComponent {
  glm::vec2 start_pos;
  glm::vec2 end_pos;
  bool is_patrolling = false;
};

struct GoalComponent {};

}  // namespace platformer

#endif  // DEMOS_GAMES_PLATFORMER_COMPONENTS_H_
