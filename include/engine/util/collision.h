/**
 * @file collision.h
 * @brief Simple 2D collision detection utilities.
 */

#ifndef INCLUDE_ENGINE_UTIL_COLLISION_H_
#define INCLUDE_ENGINE_UTIL_COLLISION_H_

#include <glm/vec2.hpp>

namespace engine::util {

/**
 * @brief Performs an Axis-Aligned Bounding Box (AABB) collision check.
 *
 * @param pos1 Position of the first box (bottom-left).
 * @param size1 Size of the first box (width, height).
 * @param pos2 Position of the second box (bottom-left).
 * @param size2 Size of the second box (width, height).
 * @return True if the boxes intersect, false otherwise.
 */
[[nodiscard]] inline bool CheckAABB(const glm::vec2& pos1,
                                    const glm::vec2& size1,
                                    const glm::vec2& pos2,
                                    const glm::vec2& size2) {
  return pos1.x < pos2.x + size2.x && pos1.x + size1.x > pos2.x &&
         pos1.y < pos2.y + size2.y && pos1.y + size1.y > pos2.y;
}

}  // namespace engine::util

#endif  // INCLUDE_ENGINE_UTIL_COLLISION_H_
