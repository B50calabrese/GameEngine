#include "engine.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <memory>

#include "window.h"

namespace engine {
namespace {
constexpr int kGlfwVersionMajor = 3;
constexpr int kGlfwVersionMinor = 3;
}  // namespace

std::unique_ptr<Window> Engine::internal_window = nullptr;

void Engine::Init(const EngineConfig& engine_config) {
  if (!glfwInit()) {
  }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, kGlfwVersionMajor);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, kGlfwVersionMinor);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  Engine::internal_window = std::make_unique<Window>(
      engine_config.window_width, engine_config.window_height,
      engine_config.window_title);
}

void Engine::Shutdown() {}

}  // namespace engine