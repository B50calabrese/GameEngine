/**
 * @file ai_components.h
 * @brief ECS components for AI (FSM and Behavior Tree).
 */

#ifndef INCLUDE_ENGINE_ECS_AI_COMPONENTS_H_
#define INCLUDE_ENGINE_ECS_AI_COMPONENTS_H_

#include <engine/util/behavior_tree.h>
#include <engine/util/fsm.h>

namespace engine::ecs {

/**
 * @brief Component that holds a StateMachine.
 */
struct StateMachineComponent {
  util::StateMachine fsm;
};

/**
 * @brief Component that holds a BehaviorTree.
 */
struct BehaviorTreeComponent {
  util::BehaviorTree tree;
};

}  // namespace engine::ecs

#endif  // INCLUDE_ENGINE_ECS_AI_COMPONENTS_H_
