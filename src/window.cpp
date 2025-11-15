// Copyright 2024 Jules Developer
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

#include "window.h"

#include <GLFW/glfw3.h>

#include <string>

#include "input_manager.h"

namespace engine {

Window::Window(const int width, const int height, const std::string& name) {
  window_ = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);

  glfwMakeContextCurrent(window_);
  glfwSwapInterval(1);  // Enable V-Sync (swap interval 1)

  SetupCallbacks();
}

void Window::PollEvents() {
  InputManager::Get().UpdateState();
  glfwPollEvents();
}

// Private functions

void Window::SetupCallbacks() {
  // 1. GLFW Key Callback
  glfwSetKeyCallback(
      window_,
      [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        // Forward the raw event to the InputManager singleton
        InputManager::Get().HandleKey(key, action);
      });

  // 2. GLFW Mouse Button Callback
  glfwSetMouseButtonCallback(
      window_, [](GLFWwindow* window, int button, int action, int mods) {
        InputManager::Get().HandleMouseButton(button, action);
      });

  // 3. GLFW Cursor Position Callback
  glfwSetCursorPosCallback(window_,
                           [](GLFWwindow* window, double xpos, double ypos) {
                             InputManager::Get().HandleCursorPosition(xpos, ypos);
                           });
}

}  // namespace engine
