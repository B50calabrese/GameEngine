/**
 * @file input_manager.h
 * @brief Handles keyboard and mouse input.
 */

#ifndef INCLUDE_ENGINE_INPUT_INPUT_MANAGER_H_
#define INCLUDE_ENGINE_INPUT_INPUT_MANAGER_H_

#include <map>

#include <glm/glm.hpp>

class Window;

namespace engine {

/**
 * @brief An enumeration of all supported keyboard and mouse button codes.
 */
enum class KeyCode {
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
  kEscape,     // Crucial for menus, pausing, or quitting
  kEnter,      // Confirming actions
  kSpace,      // Common action key (e.g., end turn, skip dialog)
  kTab,        // Toggling UI panels/map view
  kLeftShift,  // Holding for multiple selection or speed
  kLeftControl,
  kLeftAlt,
  kBackspace,  // Text input cleanup
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

  kTilde,  // For console toggle
};

/**
 * @brief A singleton class that manages all keyboard and mouse input.
 *
 * It provides a simple interface for checking the state of keys and the mouse.
 */
class InputManager {
 public:
  /**
   * @brief Returns a reference to the singleton `InputManager` instance.
   * @return Reference to the InputManager.
   */
  static InputManager& Get();

  /**
   * @brief Checks if a key is currently being held down.
   *
   * @param key_code The `KeyCode` to check.
   * @return `true` if the key is currently pressed, `false` otherwise.
   */
  bool IsKeyDown(KeyCode key_code) const;

  /**
   * @brief Checks if a key was pressed during the current frame.
   *
   * This is a single-frame event.
   *
   * @param key_code The `KeyCode` to check.
   * @return `true` if the key was pressed in this frame, `false` otherwise.
   */
  bool IsKeyPressed(KeyCode key_code) const;

  /**
   * @brief Checks if a key was released during the current frame.
   *
   * This is a single-frame event.
   *
   * @param key_code The `KeyCode` to check.
   * @return `true` if the key was released in this frame, `false` otherwise.
   */
  bool IsKeyReleased(KeyCode key_code) const;

  /**
   * @brief Retrieves the mouse position in screen coordinates.
   *
   * @returns the screen position in pixel space.
   */
  glm::vec2 mouse_screen_pos() const { return glm::vec2(mouse_x_, mouse_y_); }

  /**
   * @brief Returns true if the input has been consumed by the UI system.
   * @return `true` if consumed.
   */
  bool IsConsumed() const { return is_consumed_; }

  /**
   * @brief Marks the input as consumed for the current frame.
   */
  void Consume() { is_consumed_ = true; }

  /**
   * @brief Updates the key states for the current frame.
   *
   * This should be called once per frame by the main application loop.
   */
  void UpdateState();

  /**
   * @brief Retrieves the characters typed in the current frame.
   *
   * @return A string containing the characters.
   */
  const std::string& text_input() const { return text_input_; }

  /**
   * @brief Clears the text input buffer.
   */
  void ClearTextInput() { text_input_.clear(); }

  friend class Window;

 private:
  // Private Constructor/Deleters for Singleton pattern
  InputManager() = default;
  ~InputManager() = default;
  InputManager(const InputManager&) = delete;
  InputManager& operator=(const InputManager&) = delete;

  /**
   * @brief Raw event handler called by GLFW callbacks for key events.
   */
  void HandleKey(int raw_key_code, int action);
  /**
   * @brief Raw event handler called by GLFW callbacks for mouse button events.
   */
  void HandleMouseButton(int raw_button_code, int action);
  /**
   * @brief Raw event handler called by GLFW callbacks for cursor position
   * events.
   */
  void HandleCursorPosition(double xpos, double ypos);

  /**
   * @brief Raw event handler called by GLFW callbacks for character input.
   */
  void HandleChar(unsigned int codepoint);

  void HandleResize(int width, int height) {
    window_height_ = static_cast<float>(height);
  }

  float mouse_x_ = 0.0f;
  float mouse_y_ = 0.0f;
  float window_height_ = 600.0f;
  bool is_consumed_ = false;
  std::string text_input_;
  std::map<KeyCode, bool> current_key_state_;
  std::map<KeyCode, bool> previous_key_state_;

  /**
   * @brief Utility function to map GLFW codes to our internal KeyCode.
   */
  KeyCode MapRawCode(int raw_code) const;
};

}  // namespace engine

#endif  // INCLUDE_ENGINE_INPUT_INPUT_MANAGER_H_
