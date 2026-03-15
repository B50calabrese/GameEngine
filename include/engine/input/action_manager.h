/**
 * @file action_manager.h
 * @brief Decouples physical keys from logical gameplay intents.
 */

#ifndef INCLUDE_ENGINE_INPUT_ACTION_MANAGER_H_
#define INCLUDE_ENGINE_INPUT_ACTION_MANAGER_H_

#include <string>
#include <unordered_map>
#include <vector>

#include <engine/input/input_manager.h>

namespace engine {

/**
 * @brief State transitions for logical actions.
 */
enum class ActionState {
  None,
  Started,  // Triggered on the first frame of activation
  Ongoing,  // Active as long as the input is held
  Released  // Triggered on the frame the input is stopped
};

/**
 * @brief ActionManager decouples physical keys from logical gameplay intents.
 */
class ActionManager {
 public:
  static ActionManager& Get();
  ActionManager(const ActionManager&) = delete;
  ActionManager& operator=(const ActionManager&) = delete;

  void BindAction(const std::string& action_name, KeyCode key_code);
  bool IsStarted(const std::string& action_name) const;
  bool IsOngoing(const std::string& action_name) const;
  bool IsReleased(const std::string& action_name) const;

  bool IsConsumed() const;
  void Consume();

  void Update();

 private:
  ActionManager() = default;
  struct ActionData {
    std::vector<KeyCode> bindings;
    bool is_active_this_frame = false;
    bool was_active_last_frame = false;
  };
  std::unordered_map<std::string, ActionData> registry_;
};

}  // namespace engine

#endif  // INCLUDE_ENGINE_INPUT_ACTION_MANAGER_H_
