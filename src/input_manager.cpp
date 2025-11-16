#include "input_manager.h"

#include <GLFW/glfw3.h>

#include <iostream>

namespace engine {
namespace {
static const std::map<int, KeyCode>& GetKeyCodeMap() {
  // The map is static, meaning it's initialized only once
  // when this function is first called (thread-safe since C++11).
  static const std::map<int, KeyCode> key_code_map = {

      // --- Mouse Buttons (GLFW uses 0, 1, 2 for buttons) ---
      {GLFW_MOUSE_BUTTON_LEFT, KeyCode::kMouseLeft},
      {GLFW_MOUSE_BUTTON_RIGHT, KeyCode::kMouseRight},
      {GLFW_MOUSE_BUTTON_MIDDLE, KeyCode::kMouseMiddle},

      // --- Alphabet Keys (Sequential, but mapped explicitly for safety) ---
      {GLFW_KEY_A, KeyCode::kA},
      {GLFW_KEY_B, KeyCode::kB},
      {GLFW_KEY_C, KeyCode::kC},
      {GLFW_KEY_D, KeyCode::kD},
      {GLFW_KEY_E, KeyCode::kE},
      {GLFW_KEY_F, KeyCode::kF},
      {GLFW_KEY_G, KeyCode::kG},
      {GLFW_KEY_H, KeyCode::kH},
      {GLFW_KEY_I, KeyCode::kI},
      {GLFW_KEY_J, KeyCode::kJ},
      {GLFW_KEY_K, KeyCode::kK},
      {GLFW_KEY_L, KeyCode::kL},
      {GLFW_KEY_M, KeyCode::kM},
      {GLFW_KEY_N, KeyCode::kN},
      {GLFW_KEY_O, KeyCode::kO},
      {GLFW_KEY_P, KeyCode::kP},
      {GLFW_KEY_Q, KeyCode::kQ},
      {GLFW_KEY_R, KeyCode::kR},
      {GLFW_KEY_S, KeyCode::kS},
      {GLFW_KEY_T, KeyCode::kT},
      {GLFW_KEY_U, KeyCode::kU},
      {GLFW_KEY_V, KeyCode::kV},
      {GLFW_KEY_W, KeyCode::kW},
      {GLFW_KEY_X, KeyCode::kX},
      {GLFW_KEY_Y, KeyCode::kY},
      {GLFW_KEY_Z, KeyCode::kZ},

      // --- Number Keys (Top Row) ---
      {GLFW_KEY_0, KeyCode::k0},
      {GLFW_KEY_1, KeyCode::k1},
      {GLFW_KEY_2, KeyCode::k2},
      {GLFW_KEY_3, KeyCode::k3},
      {GLFW_KEY_4, KeyCode::k4},
      {GLFW_KEY_5, KeyCode::k5},
      {GLFW_KEY_6, KeyCode::k6},
      {GLFW_KEY_7, KeyCode::k7},
      {GLFW_KEY_8, KeyCode::k8},
      {GLFW_KEY_9, KeyCode::k9},

      // --- Functional Keys ---
      {GLFW_KEY_ESCAPE, KeyCode::kEscape},
      {GLFW_KEY_ENTER, KeyCode::kEnter},
      {GLFW_KEY_SPACE, KeyCode::kSpace},
      {GLFW_KEY_TAB, KeyCode::kTab},
      {GLFW_KEY_LEFT_SHIFT, KeyCode::kLeftShift},
      {GLFW_KEY_LEFT_CONTROL, KeyCode::kLeftControl},
      {GLFW_KEY_LEFT_ALT, KeyCode::kLeftAlt},
      {GLFW_KEY_BACKSPACE, KeyCode::kBackspace},
      {GLFW_KEY_DELETE, KeyCode::kDelete},

      // --- Navigation/Movement Keys ---
      {GLFW_KEY_UP, KeyCode::kUp},
      {GLFW_KEY_DOWN, KeyCode::kDown},
      {GLFW_KEY_LEFT, KeyCode::kLeft},
      {GLFW_KEY_RIGHT, KeyCode::kRight},

      // --- System/Function Keys ---
      {GLFW_KEY_F1, KeyCode::kF1},
      {GLFW_KEY_F2, KeyCode::kF2},
      {GLFW_KEY_F3, KeyCode::kF3},
      {GLFW_KEY_F4, KeyCode::kF4},
      {GLFW_KEY_F5, KeyCode::kF5},
      {GLFW_KEY_F6, KeyCode::kF6},
      {GLFW_KEY_F7, KeyCode::kF7},
      {GLFW_KEY_F8, KeyCode::kF8},
      {GLFW_KEY_F9, KeyCode::kF9},
      {GLFW_KEY_F10, KeyCode::kF10},
      {GLFW_KEY_F11, KeyCode::kF11},
      {GLFW_KEY_F12, KeyCode::kF12},

      // --- Utility Keys ---
      {GLFW_KEY_HOME, KeyCode::kHome},
      {GLFW_KEY_END, KeyCode::kEnd},
      {GLFW_KEY_PAGE_UP, KeyCode::kPageUp},
      {GLFW_KEY_PAGE_DOWN, KeyCode::kPageDown}};
  return key_code_map;
}
}  // namespace

InputManager& InputManager::Get() {
  static InputManager instance;
  return instance;
}

bool InputManager::IsKeyDown(const KeyCode key_code) const {
  if (current_key_state_.find(key_code) == current_key_state_.end()) {
    return false;
  }
  return current_key_state_.at(key_code);
}

bool InputManager::IsKeyPressed(const KeyCode key_code) const {
  // True if key is DOWN now (Current) AND was UP last frame (Previous)
  // Assumes keys are initialized to false in the map.
  auto current_it = current_key_state_.find(key_code);
  auto previous_it = previous_key_state_.find(key_code);

  const bool is_current_down =
      (current_it != current_key_state_.end()) ? current_it->second : false;
  const bool was_previous_down =
      (previous_it != previous_key_state_.end()) ? previous_it->second : false;
  return is_current_down && !was_previous_down;
}

bool InputManager::IsKeyReleased(const KeyCode key_code) const {
  // True if key is UP now (Current) AND was DOWN last frame (Previous)
  auto current_it = current_key_state_.find(key_code);
  auto previous_it = previous_key_state_.find(key_code);

  const bool is_current_down =
      (current_it != current_key_state_.end()) ? current_it->second : false;
  const bool was_previous_down =
      (previous_it != previous_key_state_.end()) ? previous_it->second : false;

  return !is_current_down && was_previous_down;
}

void InputManager::UpdateState() {
  previous_key_state_ = current_key_state_;
}

// Private functions

void InputManager::HandleKey(const int raw_key_code, const int action) {
  const KeyCode key = MapRawCode(raw_key_code);
  if (action == GLFW_PRESS || action == GLFW_REPEAT) {
    current_key_state_[key] = true;
  } else if (action == GLFW_RELEASE) {
    current_key_state_[key] = false;
  }
}

void InputManager::HandleMouseButton(const int raw_button_code,
                                     const int action) {
  const KeyCode key = MapRawCode(raw_button_code);
  if (action == GLFW_PRESS || action == GLFW_REPEAT) {
    current_key_state_[key] = true;
  } else if (action == GLFW_RELEASE) {
    current_key_state_[key] = false;
  }
}

void InputManager::HandleCursorPosition(const double xpos, const double ypos) {
  mouse_x_ = static_cast<float>(xpos);
  mouse_y_ = static_cast<float>(ypos);
}

// The function that performs the mapping lookup
KeyCode InputManager::MapRawCode(const int raw_code) const {
  const auto& map = GetKeyCodeMap();
  auto it = map.find(raw_code);
  if (it != map.end()) {
    return it->second;
  }
  return KeyCode::kUnknown;
}

}  // namespace engine
