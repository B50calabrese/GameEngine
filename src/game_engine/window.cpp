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

#include "game_engine/window.h"

#include <GLFW/glfw3.h>

#include <string>

#include "game_engine/input_manager.h"

namespace game_engine {

Window::Window(int width, int height, const std::string& title) {
  window_ = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
  glfwMakeContextCurrent(window_);
  glfwSwapInterval(1);
  last_frame_time_ = static_cast<float>(glfwGetTime());
  SetupCallbacks();
}

void Window::PollEvents() {
  InputManager::Get().UpdateState();
  glfwPollEvents();
}

void Window::SetupCallbacks() {
  glfwSetKeyCallback(
      window_,
      [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        InputManager::Get().HandleKey(key, action);
      });

  glfwSetMouseButtonCallback(
      window_, [](GLFWwindow* window, int button, int action, int mods) {
        InputManager::Get().HandleMouseButton(button, action);
      });

  glfwSetCursorPosCallback(window_,
                           [](GLFWwindow* window, double xpos, double ypos) {
                             InputManager::Get().HandleCursorPosition(xpos, ypos);
                           });
}

}  // namespace game_engine
