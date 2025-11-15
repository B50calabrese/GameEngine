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

#ifndef WINDOW_H
#define WINDOW_H

#include <string>

// Forward declaration
struct GLFWwindow;

namespace engine {

// The Window class is responsible for creating and managing the main
// application window.
class Window {
 public:
  // @brief Creates a new window.
  // @param width The width of the window.
  // @param height The height of the window.
  // @param name The title of the window.
  Window(int width, int height, const std::string& name);

  // @brief Returns the native handle to the window.
  // @return The native handle to the window.
  GLFWwindow* GetNativeHandle() const { return window_; }

  // @brief Polls for events.
  void PollEvents();

 private:
  GLFWwindow* window_ = nullptr;

  void SetupCallbacks();

  // Prevent copy/move to enforce a single instance handled by Engine
  Window(const Window&) = delete;
  Window& operator=(const Window&) = delete;
};

}  // namespace engine

#endif  // WINDOW_H
