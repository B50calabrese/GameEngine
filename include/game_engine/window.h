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

#ifndef GAME_ENGINE_WINDOW_H_
#define GAME_ENGINE_WINDOW_H_

#include <string>

// Forward declaration of the GLFWwindow struct to avoid including the GLFW
// header in this public header file.
struct GLFWwindow;

namespace game_engine {

/**
 * @brief Manages the application window.
 *
 * This class wraps the GLFW window, handling its creation, event polling,
 * and callbacks.
 */
class Window {
 public:
  /**
   * @brief Constructs a new Window object.
   *
   * @param width The width of the window.
   * @param height The height of the window.
   * @param title The title of the window.
   */
  Window(int width, int height, const std::string& title);

  /**
   * @brief Gets the native GLFW window handle.
   *
   * @return A pointer to the GLFWwindow object.
   */
  GLFWwindow* GetNativeHandle() const { return window_; }

  /**
   * @brief Polls for window events.
   *
   * Processes all pending events, such as keyboard and mouse inputs.
   */
  void PollEvents();

 private:
  // Sets up the GLFW callbacks for the window.
  void SetupCallbacks();

  // The native GLFW window handle.
  GLFWwindow* window_;

  // The time of the last frame, used for calculating delta time.
  float last_frame_time_;

  // Prevent copy/move to enforce a single instance handled by Engine.
  Window(const Window&) = delete;
  Window& operator=(const Window&) = delete;
};

}  // namespace game_engine

#endif  // GAME_ENGINE_WINDOW_H_
