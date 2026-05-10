/**
 * @file entity_manager.h
 * @brief Defines the EntityManager for handling entity lifecycle.
 */

#ifndef INCLUDE_ENGINE_ECS_ENTITY_MANAGER_H_
#define INCLUDE_ENGINE_ECS_ENTITY_MANAGER_H_

#include <cstdint>
#include <vector>

/**
 * @namespace engine::ecs
 * @brief Namespace for the Entity Component System.
 */
namespace engine::ecs {

/** @brief Type used to uniquely identify an entity. */
using EntityID = uint32_t;

/** @brief Constant representing an invalid or null entity. */
constexpr EntityID kInvalidEntity = 0xFFFFFFFF;

/**
 * @brief Manages the allocation and deallocation of entity IDs.
 *
 * The EntityManager keeps track of active entities and reuses IDs of destroyed
 * entities to keep the ID space compact.
 */
class EntityManager {
 public:
  /**
   * @brief Creates a new entity ID.
   *
   * If any entity IDs have been previously destroyed, one of them will be
   * reused. Otherwise, a new ID is generated.
   *
   * @return The newly created EntityID.
   */
  EntityID CreateEntity();

  /**
   * @brief Destroys an entity, making its ID available for reuse.
   * @param entity The ID of the entity to destroy.
   */
  void DestroyEntity(EntityID entity);

  /**
   * @brief Checks if an entity ID is currently in use.
   * @param entity The entity ID to check.
   * @return True if the entity is active, false otherwise.
   */
  bool IsAlive(EntityID entity) const;

  /**
   * @brief Returns the high-water mark for entity IDs.
   * @return The next ID that would be allocated if no IDs were being reused.
   */
  EntityID next_id() const { return next_id_; }

  /**
   * @brief Returns the total number of active entities.
   * @return The number of active entities.
   */
  size_t GetEntityCount() const;

  /**
   * @brief Resets the EntityManager, destroying all entities.
   */
  void Clear();

 private:
  EntityID next_id_ = 0;
  std::vector<EntityID> free_entities_;
};
}  // namespace engine::ecs

#endif  // INCLUDE_ENGINE_ECS_ENTITY_MANAGER_H_
