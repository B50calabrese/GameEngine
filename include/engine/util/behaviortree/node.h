/**
 * @file node.h
 * @brief Base node, status, and blackboard for behavior trees.
 */

#ifndef INCLUDE_ENGINE_UTIL_BEHAVIOR_TREE_NODE_H_
#define INCLUDE_ENGINE_UTIL_BEHAVIOR_TREE_NODE_H_

#include <any>
#include <memory>
#include <string>
#include <unordered_map>

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

}  // namespace engine::util

#endif  // INCLUDE_ENGINE_UTIL_BEHAVIOR_TREE_NODE_H_
