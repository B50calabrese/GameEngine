/**
 * @file engine.cpp
 * @brief Engine class implementation.
 */

/**
 * @dir src
 * @brief Root source directory.
 */

/**
 * @dir src/engine
 * @brief Engine implementation.
 */

/**
 * @dir src/engine/core
 * @brief Core engine implementation.
 */

#include <engine/core/engine.h>

// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

#include <memory>

#include <engine/graphics/renderer.h>
#include <engine/util/logger.h>
#include <engine/core/window.h>

namespace engine {
namespace {
constexpr int kGlfwVersionMajor = 3;
constexpr int kGlfwVersionMinor = 3;
}  // namespace

std::unique_ptr<Window> Engine::internal_window_ = nullptr;

void Engine::Init(const EngineConfig& engine_config) {
  if (!util::Logger::Get().IsInitialized()) {
    util::Logger::Get().Init();
  }
  LOG_INFO("Initializing Engine...");

  if (!glfwInit()) {
    LOG_ERR("Failed to initialize GLFW");
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
