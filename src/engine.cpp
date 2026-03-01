#include "engine.h"

// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

#include <memory>

#include "graphics/renderer.h"
#include "window.h"

namespace engine {
namespace {
constexpr int kGlfwVersionMajor = 3;
constexpr int kGlfwVersionMinor = 3;
}  // namespace

std::unique_ptr<Window> Engine::internal_window_ = nullptr;

void Engine::Init(const EngineConfig& engine_config) {
  if (!glfwInit()) {
    return;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, kGlfwVersionMajor);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, kGlfwVersionMinor);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  Engine::internal_window_ = std::make_unique<Window>(
      engine_config.window_width, engine_config.window_height,
      engine_config.window_title);
  (graphics::Renderer::Get()).Init(*(Engine::internal_window_));
  (graphics::Renderer::Get()).set_asset_root(engine_config.asset_path);
}

void Engine::Shutdown() {}

}  // namespace engine
