/**
 * @file events.h
 * @brief Defines the event system for the ECS infrastructure.
 */

#ifndef INCLUDE_ENGINE_ECS_EVENTS_H_
#define INCLUDE_ENGINE_ECS_EVENTS_H_

#include <engine/ecs/events/component_events.h>
#include <engine/ecs/events/entity_events.h>

namespace engine::ecs {

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

}  // namespace engine::ecs

#endif  // INCLUDE_ENGINE_ECS_EVENTS_H_
