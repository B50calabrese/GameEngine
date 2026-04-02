/**
 * @file console.h
 * @brief Engine-wide developer console.
 */

#ifndef INCLUDE_ENGINE_UTIL_CONSOLE_H_
#define INCLUDE_ENGINE_UTIL_CONSOLE_H_

#include <functional>
#include <map>
#include <string>
#include <vector>

#include <engine/input/input_manager.h>

namespace engine::util {

/**
 * @brief Manages a developer console for in-game commands.
 */
class Console {
 public:
  using CommandCallback = std::function<void(const std::vector<std::string>&)>;

  static Console& Get();

  /** @brief Toggles the console's visibility. */
  void Toggle();

  /** @brief Returns true if the console is visible. */
  bool IsVisible() const { return visible_; }

  /** @brief Returns true if the console has paused the game. */
  bool IsPaused() const { return paused_; }

  /** @brief Registers a new C++ command. */
  void RegisterCommand(const std::string& name, CommandCallback callback);

  /** @brief Updates the console (handles input). */
  void Update();

  /** @brief Renders the console overlay. */
  void Render();

  /** @brief Sets whether the game should be paused when the console is open. */
  void SetPaused(bool paused) { paused_ = paused; }

  /** @brief Sets the key used to toggle the console. */
  void SetToggleKey(KeyCode key) { toggle_key_ = key; }

  /** @brief Gets the key used to toggle the console. */
  KeyCode GetToggleKey() const { return toggle_key_; }

  /** @brief Returns true if the console is visible. */
  bool is_visible() const { return visible_; }

  /** @brief Appends text to the console output. */
  void Log(const std::string& message);

 private:
  Console();
  ~Console() = default;

  void Execute(const std::string& line);

  bool visible_ = false;
  bool paused_ = false;
  KeyCode toggle_key_ = KeyCode::kTilde;
  std::string input_buffer_;
  std::vector<std::string> output_history_;
  std::map<std::string, CommandCallback> commands_;
};

}  // namespace engine::util

#endif  // INCLUDE_ENGINE_UTIL_CONSOLE_H_
