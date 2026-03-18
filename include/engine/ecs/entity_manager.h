/**
 * @file entity_manager.h
 * @brief Manages entity lifecycle and IDs.
 */

#ifndef INCLUDE_ENGINE_ECS_ENTITY_MANAGER_H_
#define INCLUDE_ENGINE_ECS_ENTITY_MANAGER_H_

#include <cstdint>
#include <vector>

namespace engine::ecs {

using EntityID = uint32_t;
constexpr EntityID INVALID_ENTITY = 0xFFFFFFFF;

/**
 * @brief Manages the entities for the user.
 *
 * Manages the creation, and
 * destruction of entity IDs for the user.
 */
class EntityManager {
 public:
  /**
   * @brief Creates an entity, returning a free one when possible.
   */
  EntityID CreateEntity();

  /**
   * @brief Marks the given entity as 'free'.
   */
  void DestroyEntity(EntityID entity);

  /**
   * @brief Returns true if the entity is allocated.
   * @param entity
   * the entity ID to check.
   * @return `true` if the entity is being used.

   */
  bool IsAlive(EntityID entity) const;

  /**
   * @brief Returns the next available ID.
   */
  EntityID next_id() const { return next_id_; }

  /**
   * @brief Resets the manager to its initial state.
   */
  void Clear();

 private:
  EntityID next_id_ = 0;
  std::vector<EntityID> free_entities_;
};
}  // namespace engine::ecs

#endif  // INCLUDE_ENGINE_ECS_ENTITY_MANAGER_H_
