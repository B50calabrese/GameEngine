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
  virtual bool Has(EntityID entity) const = 0;
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
  void Add(EntityID entity, T component) {
    data_[entity] = std::move(component);
  }

  T& Get(EntityID entity) { return data_.at(entity); }

  void Remove(EntityID entity) override { data_.erase(entity); }

  bool Has(EntityID entity) const override {
    return data_.find(entity) != data_.end();
  }

  void Clear() override { data_.clear(); }

  void NotifyRemoved(EntityID entity, Registry* registry) override;

 private:
  std::unordered_map<EntityID, T> data_;
};

/**
 * @brief Orchestrates entities and components.
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
  /** @brief Creates an entity. */
  EntityID CreateEntity() {
    EntityID entity = entity_manager_.CreateEntity();
    Publish<events::EntityCreatedEvent>({entity, this});
    return entity;
  }

  /** @brief Deletes an entity. */
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

  /** @brief Returns true if an entity is alive. */
  bool IsAlive(EntityID entity) const {
    return entity_manager_.IsAlive(entity);
  }

  /** @brief Adds a component. */
  template <typename T>
  void AddComponent(EntityID entity, T component) {
    GetOrCreateStorage<T>()->Add(entity, std::move(component));
    Publish<events::ComponentAddedEvent<T>>(
        {entity, GetComponent<T>(entity), this});
  }

  /** @brief Removes a component. */
  template <typename T>
  void RemoveComponent(EntityID entity) {
    if (HasComponent<T>(entity)) {
      Publish<events::ComponentRemovedEvent<T>>(
          {entity, GetComponent<T>(entity), this});
      GetOrCreateStorage<T>()->Remove(entity);
    }
  }

  /** @brief Retrieves a component. */
  template <typename T>
  T& GetComponent(EntityID entity) {
    return GetOrCreateStorage<T>()->Get(entity);
  }

  /** @brief Returns true if a component exists. */
  template <typename T>
  bool HasComponent(EntityID entity) const {
    const auto* storage = GetStorage<T>();
    return storage && storage->Has(entity);
  }

  /** @brief Returns a view of entities with specific components. */
  template <typename... Components>
  class View {
   public:
    View(Registry* registry) : registry_(registry) {
      if (!registry_) return;
      for (EntityID entity = 0; entity < registry_->entity_manager_.GetNextId();
           ++entity) {
        if (registry_->entity_manager_.IsAlive(entity) &&
            (registry_->HasComponent<Components>(entity) && ...)) {
          entities_.push_back(entity);
        }
      }
    }

    View() : registry_(nullptr) {}

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

  /** @brief Subscribes a listener to events. */
  template <typename T>
  void Subscribe(events::IEventListener<T>* listener, bool one_shot = false) {
    GetOrCreateDispatcher<T>()->Subscribe(listener, one_shot);
  }

  /** @brief Unsubscribes a listener. */
  template <typename T>
  void Unsubscribe(events::IEventListener<T>* listener) {
    GetOrCreateDispatcher<T>()->Unsubscribe(listener);
  }

  /** @brief Publishes an event. */
  template <typename T>
  void Publish(const T& event, bool immediate = true) {
    GetOrCreateDispatcher<T>()->Publish(event, immediate);
  }

  /** @brief Modifies a component and triggers a modified event. */
  template <typename T, typename Func>
  void PatchComponent(EntityID entity, Func&& func) {
    if (HasComponent<T>(entity)) {
      T& component = GetComponent<T>(entity);
      func(component);
      Publish<events::ComponentModifiedEvent<T>>({entity, component, this});
    }
  }

  /** @brief Processes queued asynchronous events. */
  void Update() {
    for (auto& [type, dispatcher] : dispatchers_) {
      dispatcher->ProcessQueue();
    }
  }

  /** @brief Clears all entities and components. */
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
  template <typename T>
  ComponentStorage<T>* GetOrCreateStorage() {
    auto type = std::type_index(typeid(T));
    if (storages_.find(type) == storages_.end()) {
      storages_[type] = std::make_unique<ComponentStorage<T>>();
    }
    return static_cast<ComponentStorage<T>*>(storages_[type].get());
  }

  template <typename T>
  const ComponentStorage<T>* GetStorage() const {
    auto type = std::type_index(typeid(T));
    auto it = storages_.find(type);
    if (it == storages_.end()) return nullptr;
    return static_cast<const ComponentStorage<T>*>(it->second.get());
  }

  template <typename T>
  EventDispatcher<T>* GetOrCreateDispatcher() {
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
