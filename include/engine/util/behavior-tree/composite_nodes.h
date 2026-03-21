/**
 * @file composite_nodes.h
 * @brief Behavior tree composite nodes (Sequence, Selector).
 */

#ifndef INCLUDE_ENGINE_UTIL_BEHAVIOR_TREE_COMPOSITE_NODES_H_
#define INCLUDE_ENGINE_UTIL_BEHAVIOR_TREE_COMPOSITE_NODES_H_

#include <vector>

#include <engine/util/behavior-tree/node.h>

namespace engine::util {

/**
 * @brief Composite node that executes children in sequence.
 * Fails if any child fails, succeeds if all children succeed.
 */
class SequenceNode : public Node {
 public:
  void AddChild(std::shared_ptr<Node> child) {
    children_.push_back(std::move(child));
  }

  NodeStatus Tick(float dt, Blackboard& blackboard) override {
    for (size_t i = current_child_; i < children_.size(); ++i) {
      NodeStatus status = children_[i]->Tick(dt, blackboard);
      if (status != NodeStatus::SUCCESS) {
        if (status == NodeStatus::RUNNING) {
          current_child_ = i;
        } else {
          Reset();
        }
        return status;
      }
    }
    Reset();
    return NodeStatus::SUCCESS;
  }

  void Reset() override {
    current_child_ = 0;
    for (auto& child : children_) {
      child->Reset();
    }
  }

 private:
  std::vector<std::shared_ptr<Node>> children_;
  size_t current_child_ = 0;
};

/**
 * @brief Composite node that executes children until one succeeds.
 * Succeeds if any child succeeds, fails if all children fail.
 */
class SelectorNode : public Node {
 public:
  void AddChild(std::shared_ptr<Node> child) {
    children_.push_back(std::move(child));
  }

  NodeStatus Tick(float dt, Blackboard& blackboard) override {
    for (size_t i = current_child_; i < children_.size(); ++i) {
      NodeStatus status = children_[i]->Tick(dt, blackboard);
      if (status != NodeStatus::FAILURE) {
        if (status == NodeStatus::RUNNING) {
          current_child_ = i;
        } else {
          Reset();
        }
        return status;
      }
    }
    Reset();
    return NodeStatus::FAILURE;
  }

  void Reset() override {
    current_child_ = 0;
    for (auto& child : children_) {
      child->Reset();
    }
  }

 private:
  std::vector<std::shared_ptr<Node>> children_;
  size_t current_child_ = 0;
};

}  // namespace engine::util

#endif  // INCLUDE_ENGINE_UTIL_BEHAVIOR_TREE_COMPOSITE_NODES_H_
