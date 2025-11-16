#pragma once

#include <map>

class Window;

namespace engine {

// KeyCode is an enum that represents all the possible keys that can be pressed.
enum class KeyCode {
  kUnknown = -1,
  // --- Mouse Buttons ---
  kMouseLeft = 0,
  kMouseRight = 1,
  kMouseMiddle = 2,

  // --- Alphabet Keys (Essential for Debug/Cheats/Chat) ---
  kA,
  kB,
  kC,
  kD,
  kE,
  kF,
  kG,
  kH,
  kI,
  kJ,
  kK,
  kL,
  kM,
  kN,
  kO,
  kP,
  kQ,
  kR,
  kS,
  kT,
  kU,
  kV,
  kW,
  kX,
  kY,
  kZ,

  // --- Number Keys (Top Row) ---
  k0,
  k1,
  k2,
  k3,
  k4,
  k5,
  k6,
  k7,
  k8,
  k9,

  // --- Functional Keys (Essential Game Controls) ---
  kEscape,
  kEnter,
  kSpace,
  kTab,
  kLeftShift,
  kLeftControl,
  kLeftAlt,
  kBackspace,
  kDelete,

  // --- Navigation/Movement Keys (Even for 2D map movement) ---
  kUp,
  kDown,
  kLeft,
  kRight,

  // --- System Keys (Useful for debugging) ---
  kF1,
  kF2,
  kF3,
  kF4,
  kF5,
  kF6,
  kF7,
  kF8,
  kF9,
  kF10,
  kF11,
  kF12,

  // --- Utility Keys ---
  kHome,
  kEnd,
  kPageUp,
  kPageDown,
};

// The InputManager class is responsible for handling all input from the user.
// It is a singleton class that can be accessed from anywhere in the engine.
class InputManager {
 public:
  // @brief Returns the singleton instance of the InputManager.
  static InputManager& Get();

  // @brief Returns true if the given key is currently being pressed.
  // @param key_code The key to check.
  // @return True if the key is currently being pressed, false otherwise.
  bool IsKeyDown(KeyCode key_code) const;

  // @brief Returns true if the given key was pressed this frame.
  // @param key_code The key to check.
  // @return True if the key was pressed this frame, false otherwise.
  bool IsKeyPressed(KeyCode key_code) const;

  // @brief Returns true if the given key was released this frame.
  // @param key_code The key to check.
  // @return True if the key was released this frame, false otherwise.
  bool IsKeyReleased(KeyCode key_code) const;

  // @brief Returns the current x position of the mouse.
  // @return The current x position of the mouse.
  float GetMouseX() const { return mouse_x_; }

  // @brief Returns the current y position of the mouse.
  // @return The current y position of the mouse.
  float GetMouseY() const { return mouse_y_; }

  // @brief Updates the state of the input manager. This should be called once
  // per frame.
  void UpdateState();

  friend class Window;

 private:
  // Private Constructor/Deleters for Singleton pattern
  InputManager() = default;
  ~InputManager() = default;
  InputManager(const InputManager&) = delete;
  InputManager& operator=(const InputManager&) = delete;

  // Raw event handlers called by GLFW callbacks
  void HandleKey(int raw_key_code, int action);
  void HandleMouseButton(int raw_button_code, int action);
  void HandleCursorPosition(double xpos, double ypos);

  float mouse_x_ = 0.0f;
  float mouse_y_ = 0.0f;
  std::map<KeyCode, bool> current_key_state_;
  std::map<KeyCode, bool> previous_key_state_;

  // Utility function to map GLFW codes to our internal KeyCode
  KeyCode MapRawCode(int raw_code) const;
};

}  // namespace engine
