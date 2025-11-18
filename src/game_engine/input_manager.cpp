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

#include "game_engine/input_manager.h"

#include <GLFW/glfw3.h>

#include <iostream>

namespace game_engine {
namespace {

const std::map<int, KeyCode>& GetKeyCodeMap() {
  static const std::map<int, KeyCode> key_code_map = {
      // Mouse Buttons
      {GLFW_MOUSE_BUTTON_LEFT, KeyCode::kMouseLeft},
      {GLFW_MOUSE_BUTTON_RIGHT, KeyCode::kMouseRight},
      {GLFW_MOUSE_BUTTON_MIDDLE, KeyCode::kMouseMiddle},

      // Alphabet Keys
      {GLFW_KEY_A, KeyCode::kKeyA},
      {GLFW_KEY_B, KeyCode::kKeyB},
      {GLFW_KEY_C, KeyCode::kKeyC},
      {GLFW_KEY_D, KeyCode::kKeyD},
      {GLFW_KEY_E, KeyCode::kKeyE},
      {GLFW_KEY_F, KeyCode::kKeyF},
      {GLFW_KEY_G, KeyCode::kKeyG},
      {GLFW_KEY_H, KeyCode::kKeyH},
      {GLFW_KEY_I, KeyCode::kKeyI},
      {GLFW_KEY_J, KeyCode::kKeyJ},
      {GLFW_KEY_K, KeyCode::kKeyK},
      {GLFW_KEY_L, KeyCode::kKeyL},
      {GLFW_KEY_M, KeyCode::kKeyM},
      {GLFW_KEY_N, KeyCode::kKeyN},
      {GLFW_KEY_O, KeyCode::kKeyO},
      {GLFW_KEY_P, KeyCode::kKeyP},
      {GLFW_KEY_Q, KeyCode::kKeyQ},
      {GLFW_KEY_R, KeyCode::kKeyR},
      {GLFW_KEY_S, KeyCode::kKeyS},
      {GLFW_KEY_T, KeyCode::kKeyT},
      {GLFW_KEY_U, KeyCode::kKeyU},
      {GLFW_KEY_V, KeyCode::kKeyV},
      {GLFW_KEY_W, KeyCode::kKeyW},
      {GLFW_KEY_X, KeyCode::kKeyX},
      {GLFW_KEY_Y, KeyCode::kKeyY},
      {GLFW_KEY_Z, KeyCode::kKeyZ},

      // Number Keys
      {GLFW_KEY_0, KeyCode::kKey0},
      {GLFW_KEY_1, KeyCode::kKey1},
      {GLFW_KEY_2, KeyCode::kKey2},
      {GLFW_KEY_3, KeyCode::kKey3},
      {GLFW_KEY_4, KeyCode::kKey4},
      {GLFW_KEY_5, KeyCode::kKey5},
      {GLFW_KEY_6, KeyCode::kKey6},
      {GLFW_KEY_7, KeyCode::kKey7},
      {GLFW_KEY_8, KeyCode::kKey8},
      {GLFW_KEY_9, KeyCode::kKey9},

      // Functional Keys
      {GLFW_KEY_ESCAPE, KeyCode::kKeyEscape},
      {GLFW_KEY_ENTER, KeyCode::kKeyEnter},
      {GLFW_KEY_SPACE, KeyCode::kKeySpace},
      {GLFW_KEY_TAB, KeyCode::kKeyTab},
      {GLFW_KEY_LEFT_SHIFT, KeyCode::kKeyLeftShift},
      {GLFW_KEY_LEFT_CONTROL, KeyCode::kKeyLeftControl},
      {GLFW_KEY_LEFT_ALT, KeyCode::kKeyLeftAlt},
      {GLFW_KEY_BACKSPACE, KeyCode::kKeyBackspace},
      {GLFW_KEY_DELETE, KeyCode::kKeyDelete},

      // Navigation Keys
      {GLFW_KEY_UP, KeyCode::kKeyUp},
      {GLFW_KEY_DOWN, KeyCode::kKeyDown},
      {GLFW_KEY_LEFT, KeyCode::kKeyLeft},
      {GLFW_KEY_RIGHT, KeyCode::kKeyRight},

      // System Keys
      {GLFW_KEY_F1, KeyCode::kKeyF1},
      {GLFW_KEY_F2, KeyCode::kKeyF2},
      {GLFW_KEY_F3, KeyCode::kKeyF3},
      {GLFW_KEY_F4, KeyCode::kKeyF4},
      {GLFW_KEY_F5, KeyCode::kKeyF5},
      {GLFW_KEY_F6, KeyCode::kKeyF6},
      {GLFW_KEY_F7, KeyCode::kKeyF7},
      {GLFW_KEY_F8, KeyCode::kKeyF8},
      {GLFW_KEY_F9, KeyCode::kKeyF9},
      {GLFW_KEY_F10, KeyCode::kKeyF10},
      {GLFW_KEY_F11, KeyCode::kKeyF11},
      {GLFW_KEY_F12, KeyCode::kKeyF12},

      // Utility Keys
      {GLFW_KEY_HOME, KeyCode::kKeyHome},
      {GLFW_KEY_END, KeyCode::kKeyEnd},
      {GLFW_KEY_PAGE_UP, KeyCode::kKeyPageUp},
      {GLFW_KEY_PAGE_DOWN, KeyCode::kKeyPageDown},
  };
  return key_code_map;
}

}  // namespace

InputManager::InputManager() : mouse_x_(0.0f), mouse_y_(0.0f) {}

InputManager& InputManager::Get() {
  static InputManager instance;
  return instance;
}

bool InputManager::IsKeyDown(KeyCode key_code) const {
  const auto it = current_key_state_.find(key_code);
  if (it == current_key_state_.end()) {
    return false;
  }
  return it->second;
}

bool InputManager::IsKeyPressed(KeyCode key_code) const {
  const auto current_it = current_key_state_.find(key_code);
  const bool is_current_down =
      (current_it != current_key_state_.end()) ? current_it->second : false;

  const auto previous_it = previous_key_state_.find(key_code);
  const bool was_previous_down =
      (previous_it != previous_key_state_.end()) ? previous_it->second : false;

  return is_current_down && !was_previous_down;
}

bool InputManager::IsKeyReleased(KeyCode key_code) const {
  const auto current_it = current_key_state_.find(key_code);
  const bool is_current_down =
      (current_it != current_key_state_.end()) ? current_it->second : false;

  const auto previous_it = previous_key_state_.find(key_code);
  const bool was_previous_down =
      (previous_it != previous_key_state_.end()) ? previous_it->second : false;

  return !is_current_down && was_previous_down;
}

void InputManager::UpdateState() { previous_key_state_ = current_key_state_; }

void InputManager::HandleKey(int raw_key_code, int action) {
  const KeyCode key = MapRawCode(raw_key_code);
  if (action == GLFW_PRESS || action == GLFW_REPEAT) {
    current_key_state_[key] = true;
  } else if (action == GLFW_RELEASE) {
    current_key_state_[key] = false;
  }
}

void InputManager::HandleMouseButton(int raw_button_code, int action) {
  const KeyCode key = MapRawCode(raw_button_code);
  if (action == GLFW_PRESS) {
    current_key_state_[key] = true;
  } else if (action == GLFW_RELEASE) {
    current_key_state_[key] = false;
  }
}

void InputManager::HandleCursorPosition(double xpos, double ypos) {
  mouse_x_ = static_cast<float>(xpos);
  mouse_y_ = static_cast<float>(ypos);
}

KeyCode InputManager::MapRawCode(int raw_code) const {
  const auto& map = GetKeyCodeMap();
  const auto it = map.find(raw_code);
  if (it != map.end()) {
    return it->second;
  }
  return static_cast<KeyCode>(-1);
}

}  // namespace game_engine
