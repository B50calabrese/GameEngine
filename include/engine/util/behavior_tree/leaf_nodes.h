/**
 * @file leaf_nodes.h
 * @brief Behavior tree leaf nodes (Action, Condition).
 */

#ifndef INCLUDE_ENGINE_UTIL_BEHAVIOR_TREE_LEAF_NODES_H_
#define INCLUDE_ENGINE_UTIL_BEHAVIOR_TREE_LEAF_NODES_H_

#include <functional>
#include <engine/util/behavior_tree/node.h>

namespace engine::util {

/**
 * @brief Leaf node that executes a provided action.
 */
class ActionNode : public Node {
 public:
  using Action = std::function<NodeStatus(float, Blackboard&)>;
  explicit ActionNode(Action action) : action_(std::move(action)) {}

  NodeStatus Tick(float dt, Blackboard& blackboard) override {
    return action_(dt, blackboard);
  }

 private:
  Action action_;
};

/**
 * @brief Leaf node that checks a condition.
 */
class ConditionNode : public Node {
 public:
  using Condition = std::function<bool(Blackboard&)>;
  explicit ConditionNode(Condition condition) : condition_(std::move(condition)) {}

  NodeStatus Tick(float dt, Blackboard& blackboard) override {
    return condition_(blackboard) ? NodeStatus::SUCCESS : NodeStatus::FAILURE;
  }

 private:
  Condition condition_;
};

}  // namespace engine::util

#endif  // INCLUDE_ENGINE_UTIL_BEHAVIOR_TREE_LEAF_NODES_H_
