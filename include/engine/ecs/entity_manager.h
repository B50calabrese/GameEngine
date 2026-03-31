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
constexpr EntityID kInvalidEntity = 0xFFFFFFFF;

/**
 * @brief Manages the creation and destruction of entity IDs.
 */
class EntityManager {
 public:
  /** @brief Creates an entity, returning a free one when possible. */
  EntityID CreateEntity();

  /** @brief Marks the given entity as 'free'. */
  void DestroyEntity(EntityID entity);

  /** @brief Returns true if the entity is alive. */
  bool IsAlive(EntityID entity) const;

  /** @brief Returns the next available ID. */
  EntityID GetNextId() const { return next_id_; }

  /** @brief Resets the manager to its initial state. */
  void Clear();

 private:
  EntityID next_id_ = 0;
  std::vector<EntityID> free_entities_;
};

}  // namespace engine::ecs

#endif  // INCLUDE_ENGINE_ECS_ENTITY_MANAGER_H_
