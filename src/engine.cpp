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

