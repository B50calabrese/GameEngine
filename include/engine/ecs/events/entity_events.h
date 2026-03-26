/**
 * @file entity_events.h
 * @brief Defines entity lifecycle events for the ECS infrastructure.
 */

#ifndef INCLUDE_ENGINE_ECS_EVENTS_ENTITY_EVENTS_H_
#define INCLUDE_ENGINE_ECS_EVENTS_ENTITY_EVENTS_H_

#include <engine/ecs/entity_manager.h>

namespace engine::ecs {

class Registry;

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

}  // namespace engine::ecs

#endif  // INCLUDE_ENGINE_ECS_EVENTS_ENTITY_EVENTS_H_
