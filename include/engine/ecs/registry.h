/**
 * @file registry.h
 * @brief Main coordinator for ECS entities and components.
 */

#ifndef INCLUDE_ENGINE_ECS_REGISTRY_H_
#define INCLUDE_ENGINE_ECS_REGISTRY_H_

#include <functional>
#include <memory>
#include <typeindex>
#include <unordered_map>
#include <vector>

#include <engine/ecs/component_storage.h>
#include <engine/ecs/entity_manager.h>

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
  EntityID CreateEntity() { return entity_manager_.CreateEntity(); }

  /**
   * @brief Deletes the entity and associated data.
   * @param entity the ID of the entity to delete.
   */
  void DeleteEntity(EntityID entity) {
    for (auto& [type, storage] : storages_) {
      storage->Remove(entity);
    }
    entity_manager_.DestroyEntity(entity);
  }

  /**
   * @brief Indicates if an entity is alive.
   * @param entity the ID of the entity to check
   * @returns `true` if the entity is alive.
   */
  bool IsAlive(EntityID entity) const {
    return entity_manager_.IsAlive(entity);
  }

  /**
   * @brief Adds a component to the given entity.
   * @param entity the ID of the entity to add to.
   * @param component the component to add to the entity.
   */
  template <typename T>
  void AddComponent(EntityID entity, T component) {
    GetStorage<T>()->Add(entity, component);
  }

  /**
   * @brief Removes a component from the given entity.
   * @param entity the ID of the entity to remove.
   */
  template <typename T>
  void RemoveComponent(EntityID entity) {
    GetStorage<T>()->Remove(entity);
  }

  /**
   * @brief Retrieves the component for the entity.
   * @param entity the ID of the entity to fetch the component for.
   * @returns the component of the given type.
   */
  template <typename T>
  T& GetComponent(EntityID entity) {
    return GetStorage<T>()->Get(entity);
  }

  /**
   * @brief Returns `true` if the component exists on the entity.
   * @param entity the ID of the entity to check.
   * @returns `true` if the component exists.
   */
  template <typename T>
  bool HasComponent(EntityID entity) {
    return GetStorage<T>()->Has(entity);
  }

  /**
   * @brief A view of entities that have a specific set of components.
   */
  template <typename... Components>
  class View {
   public:
    View(Registry* registry) : registry_(registry) {
      if (!registry_) return;
      for (EntityID entity = 0; entity < registry_->entity_manager_.next_id();
           ++entity) {
        if (registry_->entity_manager_.IsAlive(entity) &&
            (registry_->HasComponent<Components>(entity) && ...)) {
          entities_.push_back(entity);
        }
      }
    }

    View() : registry_(nullptr) {}

    template <typename Func>
    std::vector<EntityID> Filter(Func&& predicate) {
      std::vector<EntityID> result;
      if (!registry_) return result;
      for (auto entity : entities_) {
        if (registry_->entity_manager_.IsAlive(entity) &&
            (registry_->HasComponent<Components>(entity) && ...) &&
            predicate(registry_->GetComponent<Components>(entity)...)) {
          result.push_back(entity);
        }
      }
      return result;
    }

    auto begin() { return entities_.begin(); }
    auto end() { return entities_.end(); }

   private:
    Registry* registry_;
    std::vector<EntityID> entities_;
  };

  template <typename... Components>
  View<Components...> GetView() {
    return View<Components...>(this);
  }

  template <typename... Components, typename Func>
  void ForEach(Func&& func) {
    auto view = GetView<Components...>();
    for (auto entity : view) {
      if (entity_manager_.IsAlive(entity) &&
          (HasComponent<Components>(entity) && ...)) {
        func(GetComponent<Components>(entity)...);
      }
    }
  }

 private:
  /**
   * @brief Gets the appropriate ComponentStorage for the given type.
   * @returns the ComponentStorage for the template type.
   */
  template <typename T>
  ComponentStorage<T>* GetStorage() {
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

#endif  // INCLUDE_ENGINE_ECS_REGISTRY_H_
