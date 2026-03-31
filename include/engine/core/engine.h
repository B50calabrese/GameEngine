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

namespace engine {

/**
 * @brief Holds the configuration for the engine's initialization.
 */
struct EngineConfig {
  int window_height = 720;
  int window_width = 1280;
  std::string window_title = "Title";
  std::string asset_path = "";
  bool hot_reload_enabled = false;
  KeyCode console_toggle_key = KeyCode::kTilde;
};

/**
 * @brief The static `Engine` class provides core lifecycle management.
 */
class Engine {
 public:
  /** @brief Initializes core systems. */
  static void Init(const EngineConfig& config);

  /** @brief Shuts down core systems. */
  static void Shutdown();

  /** @brief Gets the main window. */
  static Window& GetWindow() { return *window_; }

 private:
  static std::unique_ptr<Window> window_;
};

}  // namespace engine

#endif  // INCLUDE_ENGINE_CORE_ENGINE_H_
