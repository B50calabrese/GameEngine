/**
 * @file decorator_nodes.h
 * @brief Behavior tree decorator nodes (Inverter).
 */

#ifndef INCLUDE_ENGINE_UTIL_BEHAVIOR_TREE_DECORATOR_NODES_H_
#define INCLUDE_ENGINE_UTIL_BEHAVIOR_TREE_DECORATOR_NODES_H_

#include <engine/util/behaviortree/node.h>

namespace engine::util {

/**
 * @brief Decorator node that inverts the result of its child.
 */
class InverterNode : public Node {
 public:
  explicit InverterNode(std::shared_ptr<Node> child)
      : child_(std::move(child)) {}

  NodeStatus Tick(float dt, Blackboard& blackboard) override {
    NodeStatus status = child_->Tick(dt, blackboard);
    if (status == NodeStatus::kSuccess) {
      return NodeStatus::kFailure;
    }
    if (status == NodeStatus::kFailure) {
      return NodeStatus::kSuccess;
    }
    return status;
  }

  void Reset() override { child_->Reset(); }

 private:
  std::shared_ptr<Node> child_;
};

}  // namespace engine::util

#endif  // INCLUDE_ENGINE_UTIL_BEHAVIOR_TREE_DECORATOR_NODES_H_
