/**
 * @file registry.h
 * @brief Main coordinator for ECS entities and components.
 */

#ifndef INCLUDE_ENGINE_ECS_REGISTRY_H_
#define INCLUDE_ENGINE_ECS_REGISTRY_H_

#include <algorithm>
#include <functional>
#include <memory>
#include <typeindex>
#include <unordered_map>
#include <vector>

#include <engine/ecs/entity_manager.h>
#include <engine/ecs/events/events.h>

namespace engine::ecs {

class Registry;

/**
 * @brief Base interface for generic storage.
 */
class IComponentStorage {
 public:
  virtual ~IComponentStorage() = default;
  virtual void Remove(EntityID entity) = 0;
  virtual void Clear() = 0;
  virtual bool Has(EntityID entity) = 0;
  virtual void NotifyRemoved(EntityID entity, Registry* registry) = 0;

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
  void Remove(EntityID entity) override { data_.erase(entity); }

  /**
   * @brief Returns if the entity is in the storage.
   * @returns whether or not the entity is found in this storage.
   */
  bool Has(EntityID entity) override {
    return data_.find(entity) != data_.end();
  }

  /**
   * @brief Clears all components from this storage.
   */
  void Clear() override { data_.clear(); }

  /**
   * @brief Triggers a notification that a component is being removed.
   */
  void NotifyRemoved(EntityID entity, Registry* registry) override;

 private:
  std::unordered_map<EntityID, T> data_;
};

/**
 * @brief The Registry acts as the central coordinator for all ECS activities.
 *
 * It manages the lifecycle of entities, provides storage for components,
 * and facilitates communication through an internal event system. Most
 * high-level engine operations will interact with a `Registry` instance.
 */
class Registry {
  struct ListenerEntry {
    void* listener;
    bool one_shot;
    std::function<void(const void*)> callback;
  };

  class IEventDispatcher {
   public:
    virtual ~IEventDispatcher() = default;
    virtual void ProcessQueue() = 0;
    virtual void Clear() = 0;
  };

  template <typename T>
  class EventDispatcher : public IEventDispatcher {
   public:
    void Subscribe(events::IEventListener<T>* listener, bool one_shot) {
      listeners_.push_back({listener, one_shot, [listener](const void* event) {
                              listener->OnEvent(*static_cast<const T*>(event));
                            }});
    }

    void Unsubscribe(events::IEventListener<T>* listener) {
      listeners_.erase(std::remove_if(listeners_.begin(), listeners_.end(),
                                      [listener](const ListenerEntry& entry) {
                                        return entry.listener == listener;
                                      }),
                       listeners_.end());
    }

    void Publish(const T& event, bool immediate) {
      if (immediate) {
        for (auto it = listeners_.begin(); it != listeners_.end();) {
          it->callback(&event);
          if (it->one_shot) {
            it = listeners_.erase(it);
          } else {
            ++it;
          }
        }
      } else {
        queue_.push_back(event);
      }
    }

    void ProcessQueue() override {
      std::vector<T> current_queue = std::move(queue_);
      for (const auto& event : current_queue) {
        Publish(event, true);
      }
    }

    void Clear() override {
      listeners_.clear();
      queue_.clear();
    }

   private:
    std::vector<ListenerEntry> listeners_;
    std::vector<T> queue_;
  };

 public:
  /**
   * @brief Creates a new entity within this registry.
   *
   * This will allocate a new ID from the internal EntityManager and publish
   * an EntityCreatedEvent.
   *
   * @return The unique ID of the newly created entity.
   */
  EntityID CreateEntity() {
    EntityID entity = entity_manager_.CreateEntity();
    Publish<events::EntityCreatedEvent>({entity, this});
    return entity;
  }

  /**
   * @brief Deletes an entity and all its associated components.
   *
   * This will trigger ComponentRemovedEvents for all components attached to the
   * entity, followed by an EntityDestroyedEvent, and finally release the
   * entity ID.
   *
   * @param entity The ID of the entity to delete.
   */
  void DeleteEntity(EntityID entity) {
    Publish<events::EntityDestroyedEvent>({entity, this});
    for (auto& [type, storage] : storages_) {
      if (storage->Has(entity)) {
        storage->NotifyRemoved(entity, this);
      }
    }
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
   * @brief Attaches a component to an entity.
   *
   * If the entity already has a component of this type, it will be replaced.
   * Triggers a ComponentAddedEvent.
   *
   * @tparam T The type of the component.
   * @param entity The ID of the entity.
   * @param component The component instance to add.
   */
  template <typename T>
  void AddComponent(EntityID entity, T component) {
    GetStorage<T>()->Add(entity, component);
    Publish<events::ComponentAddedEvent<T>>(
        {entity, GetComponent<T>(entity), this});
  }

  /**
   * @brief Removes a component from the given entity.
   * @param entity the ID of the entity to remove.
   */
  template <typename T>
  void RemoveComponent(EntityID entity) {
    if (HasComponent<T>(entity)) {
      Publish<events::ComponentRemovedEvent<T>>(
          {entity, GetComponent<T>(entity), this});
      GetStorage<T>()->Remove(entity);
    }
  }

  /**
   * @brief Retrieves a reference to a component attached to an entity.
   *
   * @note Behavior is undefined if the entity does not have the component.
   * Use HasComponent<T>() to check beforehand if unsure.
   *
   * @tparam T The type of the component.
   * @param entity The ID of the entity.
   * @return A reference to the component.
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
   * @brief Represents a filtered subset of entities that possess a specific
   * set of component types.
   *
   * Views are the primary way to iterate over entities in systems. They are
   * lazy-evaluated upon creation.
   *
   * @tparam Components The pack of component types to filter for.
   */
  template <typename... Components>
  class View {
   public:
    View(Registry* registry) : registry_(registry) {
      if (!registry_) {
        return;
      }
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
      if (!registry_) {
        return result;
      }
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

  /**
   * @brief Executes a function for every entity that matches the given
   * component requirements.
   *
   * This is a convenience wrapper around creating a View and iterating over it.
   * The function should accept references to each of the requested component
   * types.
   *
   * Example:
   * @code
   * registry.ForEach<Transform, Sprite>([](Transform& t, Sprite& s) {
   *     // Update logic
   * });
   * @endcode
   *
   * @tparam Components The component types to filter for.
   * @tparam Func The type of the callback function.
   * @param func The callback function to execute.
   */
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

  /**
   * @brief Subscribes a listener to events of type T.
   * @param listener The listener to subscribe.
   * @param one_shot If true, the listener will be automatically unsubscribed
   * after one event.
   */
  template <typename T>
  void Subscribe(events::IEventListener<T>* listener, bool one_shot = false) {
    GetDispatcher<T>()->Subscribe(listener, one_shot);
  }

  /**
   * @brief Unsubscribes a listener from events of type T.
   * @param listener The listener to unsubscribe.
   */
  template <typename T>
  void Unsubscribe(events::IEventListener<T>* listener) {
    GetDispatcher<T>()->Unsubscribe(listener);
  }

  /**
   * @brief Publishes an event of type T.
   * @param event The event data.
   * @param immediate If true, listeners are notified immediately. If false, the
   * event is queued for the next Update() call.
   */
  template <typename T>
  void Publish(const T& event, bool immediate = true) {
    GetDispatcher<T>()->Publish(event, immediate);
  }

  /**
   * @brief Modifies a component and triggers a events::ComponentModifiedEvent.
   * @param entity The entity whose component to modify.
   * @param func A function that receives a reference to the component.
   */
  template <typename T, typename Func>
  void PatchComponent(EntityID entity, Func&& func) {
    if (HasComponent<T>(entity)) {
      T& component = GetComponent<T>(entity);
      func(component);
      Publish<events::ComponentModifiedEvent<T>>({entity, component, this});
    }
  }

  /**
   * @brief Processes all queued asynchronous events.
   *
   * This should be called once per frame, typically before or after the main
   * systems update, to ensure all deferred events are dispatched to listeners.
   */
  void Update() {
    for (auto& [type, dispatcher] : dispatchers_) {
      dispatcher->ProcessQueue();
    }
  }

  /**
   * @brief Returns the total number of active entities in the registry.
   * @return The number of active entities.
   */
  size_t GetEntityCount() const { return entity_manager_.GetEntityCount(); }

  /**
   * @brief Clears all entities and components from the registry.
   */
  void Clear() {
    for (auto& [type, storage] : storages_) {
      storage->Clear();
    }
    for (auto& [type, dispatcher] : dispatchers_) {
      dispatcher->Clear();
    }
    entity_manager_.Clear();
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

  /**
   * @brief Gets the appropriate EventDispatcher for the given type.
   * @returns the EventDispatcher for the template type.
   */
  template <typename T>
  EventDispatcher<T>* GetDispatcher() {
    auto type = std::type_index(typeid(T));
    if (dispatchers_.find(type) == dispatchers_.end()) {
      dispatchers_[type] = std::make_unique<EventDispatcher<T>>();
    }
    return static_cast<EventDispatcher<T>*>(dispatchers_[type].get());
  }

  EntityManager entity_manager_;
  std::unordered_map<std::type_index, std::unique_ptr<IComponentStorage>>
      storages_;
  std::unordered_map<std::type_index, std::unique_ptr<IEventDispatcher>>
      dispatchers_;
};

template <typename T>
void ComponentStorage<T>::NotifyRemoved(EntityID entity, Registry* registry) {
  if (Has(entity)) {
    registry->Publish<events::ComponentRemovedEvent<T>>(
        {entity, Get(entity), registry});
  }
}

}  // namespace engine::ecs

#endif  // INCLUDE_ENGINE_ECS_REGISTRY_H_
