/**
 * @file behavior_tree.h
 * @brief Behavior Tree utility main header.
 */

#ifndef INCLUDE_ENGINE_UTIL_BEHAVIOR_TREE_H_
#define INCLUDE_ENGINE_UTIL_BEHAVIOR_TREE_H_

#include <engine/util/behavior-tree/composite_nodes.h>
#include <engine/util/behavior-tree/decorator_nodes.h>
#include <engine/util/behavior-tree/leaf_nodes.h>
#include <engine/util/behavior-tree/node.h>

namespace engine::util {

/**
 * @brief Root of a behavior tree.
 */
class BehaviorTree {
 public:
  void SetRoot(std::shared_ptr<Node> root) { root_ = std::move(root); }
  Blackboard& GetBlackboard() { return blackboard_; }

  NodeStatus Tick(float dt) {
    if (!root_) {
      return NodeStatus::FAILURE;
    }
    return root_->Tick(dt, blackboard_);
  }

  void Reset() {
    if (root_) {
      root_->Reset();
    }
  }

 private:
  std::shared_ptr<Node> root_ = nullptr;
  Blackboard blackboard_;
};

}  // namespace engine::util

#endif  // INCLUDE_ENGINE_UTIL_BEHAVIOR_TREE_H_
