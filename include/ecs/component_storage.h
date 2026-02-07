#ifndef INCLUDE_ECS_COMPONENT_STORAGE_H
#define INCLUDE_ECS_COMPONENT_STORAGE_H

#include <unordered_map>

#include "ecs/entity_manager.h"

namespace engine::ecs {

/**
 * @brief Base interface for generic storage.
 */
class IComponentStorage {
 public:
  virtual ~IComponentStorage() = default;
  virtual void Remove(EntityID entity) = 0;

 protected:
  IComponentStorage() = default;
};

/**
 * @brief Implementation of the interface for a given type.
 */
template <typename T>
class ComponentStorage : public IComponentStorage {
 public:
  /**
   * @brief Stores the component for the given entity.
   * @param entity The entity to store the component on.
   * @param component The component being added.
   */
  void Add(EntityID entity, T component) { data_[entity] = component; }

  /**
   * @brief Returns the component for the given entity.
   * @returns the component.
   */
  T& Get(EntityID entity) { return data_.at(entity); }

  /**
   * @brief Removes the entity from this given storage.
   * @param entity The entity to remove.
   */
  void Remove(EntityID entity) { data_.erase(entity); }

  /**
   * @brief Returns if the entity is in the storage.
   * @returns whether or not the entity is found in this storage.
   */
  bool Has(EntityID entity) { return data.find(entity) != data.end(); }

 private:
  std::unordered_map<EntityID, T> data_;
};

}  // namespace engine::ecs

#endif  // INCLUDE_ECS_COMPONENT_STORAGE_H
