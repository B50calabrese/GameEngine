/**
 * @file entity_events.h
 * @brief Defines entity lifecycle events for the ECS infrastructure.
 */

#ifndef INCLUDE_ENGINE_ECS_EVENTS_ENTITY_EVENTS_H_
#define INCLUDE_ENGINE_ECS_EVENTS_ENTITY_EVENTS_H_

#include <engine/ecs/entity_manager.h>

namespace engine::ecs {
class Registry;
}

namespace engine::ecs::events {

/**
 * @brief Event triggered when a new entity is created.
 */
struct EntityCreatedEvent {
  EntityID entity;
  engine::ecs::Registry* registry;
};

/**
 * @brief Event triggered when an entity is about to be destroyed.
 */
struct EntityDestroyedEvent {
  EntityID entity;
  engine::ecs::Registry* registry;
};

}  // namespace engine::ecs::events

#endif  // INCLUDE_ENGINE_ECS_EVENTS_ENTITY_EVENTS_H_
