#include "input_manager.h"

#include <GLFW/glfw3.h>

#include <iostream>

namespace engine {
namespace {
static const std::map<int, KeyCode>& GetKeyCodeMap() {
  // The map is static, meaning it's initialized only once
  // when this function is first called (thread-safe since C++11).
  static const std::map<int, KeyCode> s_KeyCodeMap = {

      // --- Mouse Buttons (GLFW uses 0, 1, 2 for buttons) ---
      {GLFW_MOUSE_BUTTON_LEFT, KeyCode::KC_MOUSE_LEFT},
      {GLFW_MOUSE_BUTTON_RIGHT, KeyCode::KC_MOUSE_RIGHT},
      {GLFW_MOUSE_BUTTON_MIDDLE, KeyCode::KC_MOUSE_MIDDLE},

      // --- Alphabet Keys (Sequential, but mapped explicitly for safety) ---
      {GLFW_KEY_A, KeyCode::KC_A},
      {GLFW_KEY_B, KeyCode::KC_B},
      {GLFW_KEY_C, KeyCode::KC_C},
      {GLFW_KEY_D, KeyCode::KC_D},
      {GLFW_KEY_E, KeyCode::KC_E},
      {GLFW_KEY_F, KeyCode::KC_F},
      {GLFW_KEY_G, KeyCode::KC_G},
      {GLFW_KEY_H, KeyCode::KC_H},
      {GLFW_KEY_I, KeyCode::KC_I},
      {GLFW_KEY_J, KeyCode::KC_J},
      {GLFW_KEY_K, KeyCode::KC_K},
      {GLFW_KEY_L, KeyCode::KC_L},
      {GLFW_KEY_M, KeyCode::KC_M},
      {GLFW_KEY_N, KeyCode::KC_N},
      {GLFW_KEY_O, KeyCode::KC_O},
      {GLFW_KEY_P, KeyCode::KC_P},
      {GLFW_KEY_Q, KeyCode::KC_Q},
      {GLFW_KEY_R, KeyCode::KC_R},
      {GLFW_KEY_S, KeyCode::KC_S},
      {GLFW_KEY_T, KeyCode::KC_T},
      {GLFW_KEY_U, KeyCode::KC_U},
      {GLFW_KEY_V, KeyCode::KC_V},
      {GLFW_KEY_W, KeyCode::KC_W},
      {GLFW_KEY_X, KeyCode::KC_X},
      {GLFW_KEY_Y, KeyCode::KC_Y},
      {GLFW_KEY_Z, KeyCode::KC_Z},

      // --- Number Keys (Top Row) ---
      {GLFW_KEY_0, KeyCode::KC_0},
      {GLFW_KEY_1, KeyCode::KC_1},
      {GLFW_KEY_2, KeyCode::KC_2},
      {GLFW_KEY_3, KeyCode::KC_3},
      {GLFW_KEY_4, KeyCode::KC_4},
      {GLFW_KEY_5, KeyCode::KC_5},
      {GLFW_KEY_6, KeyCode::KC_6},
      {GLFW_KEY_7, KeyCode::KC_7},
      {GLFW_KEY_8, KeyCode::KC_8},
      {GLFW_KEY_9, KeyCode::KC_9},

      // --- Functional Keys ---
      {GLFW_KEY_ESCAPE, KeyCode::KC_ESCAPE},
      {GLFW_KEY_ENTER, KeyCode::KC_ENTER},
      {GLFW_KEY_SPACE, KeyCode::KC_SPACE},
      {GLFW_KEY_TAB, KeyCode::KC_TAB},
      {GLFW_KEY_LEFT_SHIFT, KeyCode::KC_LEFT_SHIFT},
      {GLFW_KEY_LEFT_CONTROL, KeyCode::KC_LEFT_CONTROL},
      {GLFW_KEY_LEFT_ALT, KeyCode::KC_LEFT_ALT},
      {GLFW_KEY_BACKSPACE, KeyCode::KC_BACKSPACE},
      {GLFW_KEY_DELETE, KeyCode::KC_DELETE},

      // --- Navigation/Movement Keys ---
      {GLFW_KEY_UP, KeyCode::KC_UP},
      {GLFW_KEY_DOWN, KeyCode::KC_DOWN},
      {GLFW_KEY_LEFT, KeyCode::KC_LEFT},
      {GLFW_KEY_RIGHT, KeyCode::KC_RIGHT},

      // --- System/Function Keys ---
      {GLFW_KEY_F1, KeyCode::KC_F1},
      {GLFW_KEY_F2, KeyCode::KC_F2},
      {GLFW_KEY_F3, KeyCode::KC_F3},
      {GLFW_KEY_F4, KeyCode::KC_F4},
      {GLFW_KEY_F5, KeyCode::KC_F5},
      {GLFW_KEY_F6, KeyCode::KC_F6},
      {GLFW_KEY_F7, KeyCode::KC_F7},
      {GLFW_KEY_F8, KeyCode::KC_F8},
      {GLFW_KEY_F9, KeyCode::KC_F9},
      {GLFW_KEY_F10, KeyCode::KC_F10},
      {GLFW_KEY_F11, KeyCode::KC_F11},
      {GLFW_KEY_F12, KeyCode::KC_F12},

      // --- Utility Keys ---
      {GLFW_KEY_HOME, KeyCode::KC_HOME},
      {GLFW_KEY_END, KeyCode::KC_END},
      {GLFW_KEY_PAGE_UP, KeyCode::KC_PAGE_UP},
      {GLFW_KEY_PAGE_DOWN, KeyCode::KC_PAGE_DOWN}};
  return s_KeyCodeMap;
}
}  // namespace

InputManager& InputManager::Get() {
  static InputManager instance;
  return instance;
}

bool InputManager::IsKeyDown(KeyCode key_code) const {
  if (this->current_key_state.find(key_code) == this->current_key_state.end()) {
    return false;
  }
  return this->current_key_state.at(key_code);
}

bool InputManager::IsKeyPressed(KeyCode key_code) const {
  // True if key is DOWN now (Current) AND was UP last frame (Previous)
  // Assumes keys are initialized to false in the map.
  auto current_it = this->current_key_state.find(key_code);
  auto previous_it = this->previous_key_state.find(key_code);

  bool is_current_down = (current_it != this->current_key_state.end())
                             ? current_it->second
                             : false;
  bool was_previous_down = (previous_it != this->previous_key_state.end())
                               ? previous_it->second
                               : false;
  return is_current_down && !was_previous_down;
}

bool InputManager::IsKeyReleased(KeyCode key_code) const {
  // True if key is UP now (Current) AND was DOWN last frame (Previous)
  auto current_it = this->current_key_state.find(key_code);
  auto previous_it = this->previous_key_state.find(key_code);

  bool is_current_down = (current_it != this->current_key_state.end())
                             ? current_it->second
                             : false;
  bool was_previous_down = (previous_it != this->previous_key_state.end())
                               ? previous_it->second
                               : false;

  return !is_current_down && was_previous_down;
}

void InputManager::UpdateState() {
  this->previous_key_state = this->current_key_state;
}

// Private functions

void InputManager::HandleKey(int raw_key_code, int action) {
  KeyCode key = MapRawCode(raw_key_code);
  if (action == GLFW_PRESS || action == GLFW_REPEAT) {
    this->current_key_state[key] = true;
  } else if (action == GLFW_RELEASE) {
    this->current_key_state[key] = false;
  }
}

void InputManager::HandleMouseButton(int raw_button_code, int action) {
  // Implementation here
}

void InputManager::HandleCursorPosition(double xpos, double ypos) {
  this->mouse_x = static_cast<float>(xpos);
  this->mouse_y = static_cast<float>(ypos);
}

// The function that performs the mapping lookup
KeyCode InputManager::MapRawCode(int raw_code) const {
  const auto& map = GetKeyCodeMap();

  auto it = map.find(raw_code);

  if (it != map.end()) {
    return it->second;
  }

  return static_cast<KeyCode>(-1);
}

}  // namespace engine