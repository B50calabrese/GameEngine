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
 *
 * This allows game logic to remain platform-agnostic and simplifies the
 * implementation of rebindable controls and multi-input support.
 */
class ActionManager {
 public:
  /**
   * @brief Returns a reference to the singleton `ActionManager` instance.
   * @return Reference to the ActionManager.
   */
  static ActionManager& Get();

  // Deleted for Singleton pattern rigor
  ActionManager(const ActionManager&) = delete;
  ActionManager& operator=(const ActionManager&) = delete;

  /**
   * @brief Bind a logical name to a physical input code.
   * @param action_name The name used by game logic (e.g., "Jump")
   * @param key_code The raw key code (e.g., KeyCode::KC_SPACE)
   */
  void BindAction(const std::string& action_name, KeyCode key_code);

  /**
   * @brief Returns true only on the frame the action was activated.
   * @param action_name The name of the action to check.
   * @return `true` if the action started this frame, `false` otherwise.
   */
  bool IsStarted(const std::string& action_name) const;

  /**
   * @brief Returns true as long as any bound input is active.
   * @param action_name The name of the action to check.
   * @return `true` if the action is currently active, `false` otherwise.
   */
  bool IsOngoing(const std::string& action_name) const;

  /**
   * @brief Returns true only on the frame the action was deactivated.
   * @param action_name The name of the action to check.
   * @return `true` if the action was released this frame, `false` otherwise.
   */
  bool IsReleased(const std::string& action_name) const;

  /**
   * @brief Synchronizes state with InputManager.
   *
   * Must be called at the end of the Engine frame loop.
   */
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
