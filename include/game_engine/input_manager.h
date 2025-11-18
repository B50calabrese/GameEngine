// Copyright 2024 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef GAME_ENGINE_INPUT_MANAGER_H_
#define GAME_ENGINE_INPUT_MANAGER_H_

#include <map>

namespace game_engine {

// Forward declaration for the Window class to avoid circular dependencies.
class Window;

/**
 * @brief Defines the key codes for keyboard and mouse inputs.
 */
enum class KeyCode {
  // Mouse Buttons
  kMouseLeft = 0,
  kMouseRight = 1,
  kMouseMiddle = 2,

  // Alphabet Keys
  kKeyA,
  kKeyB,
  kKeyC,
  kKeyD,
  kKeyE,
  kKeyF,
  kKeyG,
  kKeyH,
  kKeyI,
  kKeyJ,
  kKeyK,
  kKeyL,
  kKeyM,
  kKeyN,
  kKeyO,
  kKeyP,
  kKeyQ,
  kKeyR,
  kKeyS,
  kKeyT,
  kKeyU,
  kKeyV,
  kKeyW,
  kKeyX,
  kKeyY,
  kKeyZ,

  // Number Keys
  kKey0,
  kKey1,
  kKey2,
  kKey3,
  kKey4,
  kKey5,
  kKey6,
  kKey7,
  kKey8,
  kKey9,

  // Functional Keys
  kKeyEscape,
  kKeyEnter,
  kKeySpace,
  kKeyTab,
  kKeyLeftShift,
  kKeyLeftControl,
  kKeyLeftAlt,
  kKeyBackspace,
  kKeyDelete,

  // Navigation Keys
  kKeyUp,
  kKeyDown,
  kKeyLeft,
  kKeyRight,

  // System Keys
  kKeyF1,
  kKeyF2,
  kKeyF3,
  kKeyF4,
  kKeyF5,
  kKeyF6,
  kKeyF7,
  kKeyF8,
  kKeyF9,
  kKeyF10,
  kKeyF11,
  kKeyF12,

  // Utility Keys
  kKeyHome,
  kKeyEnd,
  kKeyPageUp,
  kKeyPageDown,
};

/**
 * @brief Manages all keyboard and mouse inputs.
 *
 * This class is a singleton that tracks the state of all inputs, providing
 * functions to check if a key is down, pressed, or released.
 */
class InputManager {
 public:
  /**
   * @brief Gets the singleton instance of the InputManager.
   *
   * @return A reference to the InputManager instance.
   */
  static InputManager& Get();

  /**
   * @brief Checks if a key is currently held down.
   *
   * @param key_code The key to check.
   * @return True if the key is down, false otherwise.
   */
  bool IsKeyDown(KeyCode key_code) const;

  /**
   * @brief Checks if a key was pressed in the current frame.
   *
   * @param key_code The key to check.
   * @return True if the key was pressed, false otherwise.
   */
  bool IsKeyPressed(KeyCode key_code) const;

  /**
   * @brief Checks if a key was released in the current frame.
   *
   * @param key_code The key to check.
   * @return True if the key was released, false otherwise.
   */
  bool IsKeyReleased(KeyCode key_code) const;

  /**
   * @brief Gets the current X position of the mouse cursor.
   *
   * @return The X coordinate of the mouse.
   */
  float GetMouseX() const { return mouse_x_; }

  /**
   * @brief Gets the current Y position of the mouse cursor.
   *
   * @return The Y coordinate of the mouse.
   */
  float GetMouseY() const { return mouse_y_; }

  /**
   * @brief Updates the input state for the current frame.
   *
   * This should be called once per frame to update the previous key state.
   */
  void UpdateState();

  // The Window class needs to access the private Handle* functions.
  friend class Window;

 private:
  InputManager();
  ~InputManager() = default;
  InputManager(const InputManager&) = delete;
  InputManager& operator=(const InputManager&) = delete;

  // Handles raw key events from GLFW.
  void HandleKey(int raw_key_code, int action);
  // Handles raw mouse button events from GLFW.
  void HandleMouseButton(int raw_button_code, int action);
  // Handles raw cursor position events from GLFW.
  void HandleCursorPosition(double xpos, double ypos);

  // Maps a GLFW key code to our internal KeyCode enum.
  KeyCode MapRawCode(int raw_code) const;

  float mouse_x_ = 0.0f;
  float mouse_y_ = 0.0f;
  std::map<KeyCode, bool> current_key_state_;
  std::map<KeyCode, bool> previous_key_state_;
};

}  // namespace game_engine

#endif  // GAME_ENGINE_INPUT_MANAGER_H_
