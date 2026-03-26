/**
 * @file events.h
 * @brief Defines the event system for the ECS infrastructure.
 */

#ifndef INCLUDE_ENGINE_ECS_EVENTS_H_
#define INCLUDE_ENGINE_ECS_EVENTS_H_

#include <engine/ecs/entity_manager.h>

namespace engine::ecs {

class Registry;

/**
 * @brief Base interface for event listeners.
 * @tparam T The type of event to listen for.
 */
template <typename T>
class IEventListener {
 public:
  virtual ~IEventListener() = default;

  /**
   * @brief Called when an event of type T is published.
   * @param event The event data.
   */
  virtual void OnEvent(const T& event) = 0;
};

/**
 * @brief Event triggered when a new entity is created.
 */
struct EntityCreatedEvent {
  EntityID entity;
  Registry* registry;
};

/**
 * @brief Event triggered when an entity is about to be destroyed.
 */
struct EntityDestroyedEvent {
  EntityID entity;
  Registry* registry;
};

/**
 * @brief Event triggered when a component is added to an entity.
 * @tparam T The type of component added.
 */
template <typename T>
struct ComponentAddedEvent {
  EntityID entity;
  T& component;
  Registry* registry;
};

/**
 * @brief Event triggered when a component is about to be removed from an
 * entity.
 * @tparam T The type of component removed.
 */
template <typename T>
struct ComponentRemovedEvent {
  EntityID entity;
  T& component;
  Registry* registry;
};

/**
 * @brief Event triggered when a component is modified.
 * @tparam T The type of component modified.
 */
template <typename T>
struct ComponentModifiedEvent {
  EntityID entity;
  T& component;
  Registry* registry;
};

}  // namespace engine::ecs

#endif  // INCLUDE_ENGINE_ECS_EVENTS_H_
