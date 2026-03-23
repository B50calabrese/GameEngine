#include "enemy_ai.h"

#include <algorithm>
#include <random>

#include <glm/glm.hpp>

namespace tactical_rpg {

void EnemyAI::ProcessTurn(Character* active, std::vector<Character>& party) {
  Character* closest_player = nullptr;
  int min_dist = 1000;

  for (auto& p : party) {
    if (!p.is_downed) {
      int dist = std::abs(p.grid_pos.x - active->grid_pos.x) +
                 std::abs(p.grid_pos.y - active->grid_pos.y);
      if (dist < min_dist) {
        min_dist = dist;
        closest_player = &p;
      }
    }
  }

  if (closest_player) {
    glm::ivec2 diff = closest_player->grid_pos - active->grid_pos;
    int dist_x = std::abs(diff.x);
    int dist_y = std::abs(diff.y);

    int total_speed = active->stats.speed;

    // Move as close as possible to the player within speed limits
    int move_x = std::min(total_speed, dist_x);
    if (diff.x < 0) move_x = -move_x;
    active->grid_pos.x += move_x;
    total_speed -= std::abs(move_x);

    int move_y = std::min(total_speed, dist_y);
    if (diff.y < 0) move_y = -move_y;
    active->grid_pos.y += move_y;

    auto& action = active->actions[0];
    int final_dist = std::abs(closest_player->grid_pos.x - active->grid_pos.x) +
                     std::abs(closest_player->grid_pos.y - active->grid_pos.y);
    if (final_dist <= action.range) {
      std::mt19937 gen(std::random_device{}());
      int attack_roll = std::uniform_int_distribution<int>(1, 20)(gen) + 2;
      if (attack_roll >= closest_player->stats.ac) {
        int damage = std::uniform_int_distribution<int>(
                         1, action.damage_dice_sides)(gen) *
                         action.damage_dice_count +
                     action.damage_modifier;
        closest_player->stats.current_hp -= damage;
        if (closest_player->stats.current_hp <= 0) {
          closest_player->is_downed = true;
          closest_player->stats.current_hp = 0;
        }
      }
    }
  }
}

}  // namespace tactical_rpg
