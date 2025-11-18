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

#ifndef GAME_ENGINE_ENGINE_H_
#define GAME_ENGINE_ENGINE_H_

#include <memory>
#include <string>

#include "game_engine/window.h"

namespace game_engine {

/**
 * @brief Configuration settings for the game engine.
 *
 * This struct holds all the necessary settings to initialize the engine,
 * such as window dimensions and title.
 */
struct EngineConfig {
  int window_height = 720;
  int window_width = 1280;
  std::string window_title = "Title";
};

/**
 * @brief Manages the lifecycle of the game engine.
 *
 * The Engine class is responsible for initializing and shutting down the
 * engine's core components, including the main window.
 */
class Engine {
 public:
  /**
   * @brief Initializes the game engine.
   *
   * Sets up GLFW, creates the main window, and initializes OpenGL.
   *
   * @param config The engine configuration settings.
   */
  static void Init(const EngineConfig& config);

  /**
   * @brief Shuts down the game engine.
   *
   * Performs cleanup and releases all engine resources.
   */
  static void Shutdown();

  /**
   * @brief Gets a reference to the main window.
   *
   * @return A reference to the Window instance.
   */
  static Window& GetWindow() { return *window_; }

 private:
  // The main window instance.
  static std::unique_ptr<Window> window_;
};

}  // namespace game_engine

#endif  // GAME_ENGINE_ENGINE_H_
