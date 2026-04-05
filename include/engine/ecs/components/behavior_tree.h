/**
 * @file behavior_tree.h
 * @brief Component that holds a BehaviorTree.
 */

#ifndef INCLUDE_ENGINE_ECS_COMPONENTS_BEHAVIOR_TREE_H_
#define INCLUDE_ENGINE_ECS_COMPONENTS_BEHAVIOR_TREE_H_

#include <engine/util/behavior_tree.h>

namespace engine::ecs::components {

/**
 * @brief Component that holds a BehaviorTree.
 */
struct BehaviorTree {
  util::BehaviorTree tree;
};

}  // namespace engine::ecs::components

#endif  // INCLUDE_ENGINE_ECS_COMPONENTS_BEHAVIOR_TREE_H_
