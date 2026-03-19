#ifndef DEMOS_GAMES_PLATFORMER_COMPONENTS_H_
#define DEMOS_GAMES_PLATFORMER_COMPONENTS_H_

#include <glm/vec2.hpp>

namespace platformer {

struct PlayerComponent {
  glm::vec2 position = {0.0f, 0.0f};
  glm::vec2 velocity = {0.0f, 0.0f};
  bool is_grounded = false;
  float jump_force = 600.0f;
  float move_speed = 300.0f;
  glm::vec2 size = {30.0f, 30.0f};
};

enum class PlatformType { Stationary, Moving, Temporary };

struct PlatformComponent {
  PlatformType type = PlatformType::Stationary;
  glm::vec2 position;
  glm::vec2 size;
  glm::vec2 velocity = {0.0f, 0.0f};
  glm::vec2 start_pos;
  glm::vec2 end_pos;
  float timer = 0.0f;
  bool touched = false;
  bool active = true;
};

struct EnemyComponent {
  glm::vec2 position;
  glm::vec2 size = {30.0f, 30.0f};
  glm::vec2 velocity = {100.0f, 0.0f};
  glm::vec2 start_pos;
  glm::vec2 end_pos;
  bool is_patrolling = false;
};

struct GoalComponent {
  glm::vec2 position;
  glm::vec2 size = {40.0f, 40.0f};
};

}  // namespace platformer

#endif  // DEMOS_GAMES_PLATFORMER_COMPONENTS_H_
