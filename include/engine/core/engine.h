/**
 * @file engine.h
 * @brief Main engine entry point and configuration.
 */

#ifndef INCLUDE_ENGINE_CORE_ENGINE_H_
#define INCLUDE_ENGINE_CORE_ENGINE_H_

#include <memory>
#include <string>

#include <engine/core/window.h>
#include <engine/input/input_manager.h>

/**
 * @namespace engine
 * @brief The core namespace for the Game Engine.
 */
namespace engine {

/**
 * @brief Holds the configuration for the engine's initialization.
 */
struct EngineConfig {
  /** @brief The height of the main application window. */
  int window_height = 720;
  /** @brief The width of the main application window. */
  int window_width = 1280;
  /** @brief The title of the main application window. */
  std::string window_title = "Title";
  /** @brief The default path for assets. */
  std::string asset_path = "";
  /** @brief Whether to enable Lua script hot reloading. */
  bool hot_reload_enabled = false;
  /** @brief The key used to toggle the developer console. */
  KeyCode console_toggle_key = KeyCode::kTilde;
};

/**
 * @brief The static `Engine` class provides core lifecycle management.
 */
class Engine {
 public:
  /**
   * @brief Initializes core systems.
   * @param config The engine configuration.
   */
  static void Init(const EngineConfig& config);

  /**
   * @brief Gracefully shuts down the engine.
   */
  static void Shutdown();

  /**
   * @brief Provides access to the main application window.
   * @return Reference to the main Window.
   */
  static Window& GetWindow() { return *internal_window_; }

 private:
  static std::unique_ptr<Window> internal_window_;
};
}  // namespace engine

#endif  // INCLUDE_ENGINE_CORE_ENGINE_H_
