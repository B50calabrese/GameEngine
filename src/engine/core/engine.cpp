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

#include <engine/core/job_system.h>
#include <engine/core/window.h>
#include <engine/graphics/renderer.h>
#include <engine/util/logger.h>
#include <engine/util/scripting/script_manager.h>

#include <cstdlib>

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

  core::JobSystem::Get().Init();

  // Configure scripting
  bool hot_reload = engine_config.hot_reload_enabled;
  const char* hot_reload_env = std::getenv("ENGINE_HOT_RELOAD");
  if (hot_reload_env && std::string(hot_reload_env) == "1") {
    hot_reload = true;
  }
  util::ScriptManager::Get().set_hot_reload_enabled(hot_reload);
  util::ScriptManager::Get().set_asset_path(engine_config.asset_path);
}

void Engine::Shutdown() {
  if (internal_window_) {
    glfwSetWindowShouldClose(internal_window_->native_handle(), GLFW_TRUE);
  }
  core::JobSystem::Get().Shutdown();
}

}  // namespace engine
