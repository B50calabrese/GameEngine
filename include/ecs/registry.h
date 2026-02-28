#ifndef INCLUDE_ECS_REGISTRY_H_
#define INCLUDE_ECS_REGISTRY_H_

#include <memory>
#include <typeindex>
#include <unordered_map>

#include "ecs/component_storage.h"
#include "ecs/entity_manager.h"

namespace engine::ecs {

/**
 * @brief Orchestrates the entities and components associated with those
 * entities.
 */
class Registry {
 public:
  /**
   * @brief Creates an entity in the registry.
   * @returns the ID of the entity created.
   */
  EntityID create_entity() { return entity_manager_.create_entity(); }

  /**
   * @brief Deletes the entity and associated data.
   * @param entity the ID of the entity to delete.
   */
  void delete_entity(EntityID entity) {
    for (auto& [type, storage] : storages_) {
      storage->remove(entity);
    }
    entity_manager_.destroy_entity(entity);
  }

  /**
   * @brief Indicates if an entity is alive.
   * @param entity the ID of the entity to check
   * @returns `true` if the entity is alive.
   */
  bool is_alive(EntityID entity) const {
    return entity_manager_.is_alive(entity);
  }

  /**
   * @brief Adds a component to the given entity.
   * @param entity the ID of the entity to add to.
   * @param component the component to add to the entity.
   */
  template <typename T>
  void add_component(EntityID entity, T component) {
    get_storage<T>()->add(entity, component);
  }

  /**
   * @brief Removes a component from the given entity.
   * @param entity the ID of the entity to remove.
   */
  template <typename T>
  void remove_component(EntityID entity) {
    get_storage<T>()->remove(entity);
  }

  /**
   * @brief Retrieves the component for the entity.
   * @param entity the ID of the entity to fetch the component for.
   * @returns the component of the given type.
   */
  template <typename T>
  T& get_component(EntityID entity) {
    return get_storage<T>()->get(entity);
  }

  /**
   * @brief Returns `true` if the component exists on the entity.
   * @param entity the ID of the entity to check.
   * @returns `true` if the component exists.
   */
  template <typename T>
  bool has_component(EntityID entity) {
    return get_storage<T>()->has(entity);
  }

 private:
  /**
   * @brief Gets the appropriate ComponentStorage for the given type.
   * @returns the ComponentStorage for the template type.
   */
  template <typename T>
  ComponentStorage<T>* get_storage() {
    auto type = std::type_index(typeid(T));
    if (storages_.find(type) == storages_.end()) {
      storages_[type] = std::make_unique<ComponentStorage<T>>();
    }
    return static_cast<ComponentStorage<T>*>(storages_[type].get());
  }

  EntityManager entity_manager_;
  std::unordered_map<std::type_index, std::unique_ptr<IComponentStorage>>
      storages_;
};

}  // namespace engine::ecs

#endif  // INCLUDE_ECS_REGISTRY_H_
