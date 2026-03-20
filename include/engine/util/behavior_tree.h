/**
 * @file behavior_tree.h
 * @brief Behavior Tree utility.
 */

#ifndef INCLUDE_ENGINE_UTIL_BEHAVIOR_TREE_H_
#define INCLUDE_ENGINE_UTIL_BEHAVIOR_TREE_H_

#include <any>
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace engine::util {

/**
 * @brief Status of a behavior tree node.
 */
enum class NodeStatus { SUCCESS, FAILURE, RUNNING };

/**
 * @brief Blackboard for shared data between nodes.
 */
class Blackboard {
 public:
  template <typename T>
  void Set(const std::string& key, T value) {
    data_[key] = value;
  }

  template <typename T>
  T Get(const std::string& key, T default_value = T()) const {
    auto it = data_.find(key);
    if (it != data_.end()) {
      try {
        return std::any_cast<T>(it->second);
      } catch (const std::bad_any_cast&) {
        return default_value;
      }
    }
    return default_value;
  }

  bool Has(const std::string& key) const { return data_.count(key) > 0; }
  void Remove(const std::string& key) { data_.erase(key); }

 private:
  std::unordered_map<std::string, std::any> data_;
};

/**
 * @brief Base class for a behavior tree node.
 */
class Node {
 public:
  virtual ~Node() = default;

  /**
   * @brief Executes the node logic.
   * @param dt Delta time since the last frame.
   * @param blackboard Shared data between nodes.
   * @return The status of the node execution.
   */
  virtual NodeStatus Tick(float dt, Blackboard& blackboard) = 0;

  /**
   * @brief Resets the node state.
   */
  virtual void Reset() {}
};

/**
 * @brief Composite node that executes children in sequence.
 * Fails if any child fails, succeeds if all children succeed.
 */
class SequenceNode : public Node {
 public:
  void AddChild(std::shared_ptr<Node> child) { children_.push_back(std::move(child)); }

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
  void AddChild(std::shared_ptr<Node> child) { children_.push_back(std::move(child)); }

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

/**
 * @brief Decorator node that inverts the result of its child.
 */
class InverterNode : public Node {
 public:
  explicit InverterNode(std::shared_ptr<Node> child) : child_(std::move(child)) {}

  NodeStatus Tick(float dt, Blackboard& blackboard) override {
    NodeStatus status = child_->Tick(dt, blackboard);
    if (status == NodeStatus::SUCCESS) return NodeStatus::FAILURE;
    if (status == NodeStatus::FAILURE) return NodeStatus::SUCCESS;
    return status;
  }

  void Reset() override { child_->Reset(); }

 private:
  std::shared_ptr<Node> child_;
};

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

/**
 * @brief Root of a behavior tree.
 */
class BehaviorTree {
 public:
  void SetRoot(std::shared_ptr<Node> root) { root_ = std::move(root); }
  Blackboard& GetBlackboard() { return blackboard_; }

  NodeStatus Tick(float dt) {
    if (!root_) return NodeStatus::FAILURE;
    return root_->Tick(dt, blackboard_);
  }

  void Reset() {
    if (root_) root_->Reset();
  }

 private:
  std::shared_ptr<Node> root_ = nullptr;
  Blackboard blackboard_;
};

}  // namespace engine::util

#endif  // INCLUDE_ENGINE_UTIL_BEHAVIOR_TREE_H_
