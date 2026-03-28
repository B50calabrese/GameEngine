/**
 * @file component_events.h
 * @brief Defines component lifecycle events for the ECS infrastructure.
 */

#ifndef INCLUDE_ENGINE_ECS_EVENTS_COMPONENT_EVENTS_H_
#define INCLUDE_ENGINE_ECS_EVENTS_COMPONENT_EVENTS_H_

#include <engine/ecs/entity_manager.h>

namespace engine::ecs::events {

class Registry;

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

}  // namespace engine::ecs::events

#endif  // INCLUDE_ENGINE_ECS_EVENTS_COMPONENT_EVENTS_H_
