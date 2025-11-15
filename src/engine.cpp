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

#include "engine.h"

#include <iostream>
#include <memory>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "window.h"

namespace engine {
namespace {
constexpr int kGlfwVersionMajor = 3;
constexpr int kGlfwVersionMinor = 3;
}  // namespace

std::unique_ptr<Window> Engine::window_ = nullptr;

void Engine::Init(const EngineConfig& engine_config) {
  if (!glfwInit()) {
    std::cerr << "Failed to initialize GLFW" << std::endl;
    return;
  }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, kGlfwVersionMajor);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, kGlfwVersionMinor);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window_ = std::make_unique<Window>(
      engine_config.window_width, engine_config.window_height,
      engine_config.window_title);
}

void Engine::Shutdown() {
    window_.reset();
    glfwTerminate();
}

}  // namespace engine

