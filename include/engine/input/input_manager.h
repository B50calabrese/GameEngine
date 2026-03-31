/**
 * @file input_manager.h
 * @brief Handles keyboard and mouse input.
 */

#ifndef INCLUDE_ENGINE_INPUT_INPUT_MANAGER_H_
#define INCLUDE_ENGINE_INPUT_INPUT_MANAGER_H_

#include <map>
#include <string>

#include <glm/glm.hpp>

namespace engine {

/**
 * @brief Key and mouse button codes.
 */
enum class KeyCode {
  kMouseLeft = 0,
  kMouseRight = 1,
  kMouseMiddle = 2,
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
  kEscape,
  kEnter,
  kSpace,
  kTab,
  kLeftShift,
  kLeftControl,
  kLeftAlt,
  kBackspace,
  kDelete,
  kUp,
  kDown,
  kLeft,
  kRight,
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
  kHome,
  kEnd,
  kPageUp,
  kPageDown,
  kTilde,
};

/**
 * @brief Manages keyboard and mouse input.
 */
class InputManager {
 public:
  /** @brief Gets the singleton instance. */
  static InputManager& Get();

  /** @brief Returns true if a key is down. */
  bool IsKeyDown(KeyCode key_code) const;

  /** @brief Returns true if a key was pressed this frame. */
  bool IsKeyPressed(KeyCode key_code) const;

  /** @brief Returns true if a key was released this frame. */
  bool IsKeyReleased(KeyCode key_code) const;

  /** @brief Gets mouse position in screen coordinates. */
  glm::vec2 GetMouseScreenPos() const { return {mouse_x_, mouse_y_}; }

  /** @brief Returns true if input was consumed by UI. */
  bool IsConsumed() const { return is_consumed_; }

  /** @brief Consumes input for the current frame. */
  void Consume() { is_consumed_ = true; }

  /** @brief Updates key states. Called every frame. */
  void UpdateState();

  /** @brief Gets typed characters. */
  const std::string& GetTextInput() const { return text_input_; }

  /** @brief Clears text input buffer. */
  void ClearTextInput() { text_input_.clear(); }

 private:
  friend class Window;

  InputManager() = default;
  ~InputManager() = default;

  InputManager(const InputManager&) = delete;
  InputManager& operator=(const InputManager&) = delete;

  void HandleKey(int raw_key_code, int action);
  void HandleMouseButton(int raw_button_code, int action);
  void HandleCursorPosition(double xpos, double ypos);
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

  KeyCode MapRawCode(int raw_code) const;
};

}  // namespace engine

#endif  // INCLUDE_ENGINE_INPUT_INPUT_MANAGER_H_
