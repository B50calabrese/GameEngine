/**
 * @file state_machine.h
 * @brief Component that holds a StateMachine.
 */

#ifndef INCLUDE_ENGINE_ECS_COMPONENTS_STATE_MACHINE_H_
#define INCLUDE_ENGINE_ECS_COMPONENTS_STATE_MACHINE_H_

#include <engine/util/fsm.h>

namespace engine::ecs::components {

/**
 * @brief Component that holds a StateMachine.
 */
struct StateMachine {
  util::StateMachine fsm;
};

}  // namespace engine::ecs::components

#endif  // INCLUDE_ENGINE_ECS_COMPONENTS_STATE_MACHINE_H_
