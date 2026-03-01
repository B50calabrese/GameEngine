#ifndef INCLUDE_ENGINE_H_
#define INCLUDE_ENGINE_H_

#include <memory>
#include <string>

#include "window.h"

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
};

/**
 * @brief The static `Engine` class provides the core functionality of the game
 * engine, including initialization, shutdown, and access to engine-managed
 * systems.
 */
class Engine {
 public:
  /**
   * @brief Initializes the engine, including the window, graphics context, and
   * other core systems. This must be called before any other engine functions.
   *
   * @param config The `EngineConfig` struct containing initialization
   * settings.
   */
  static void Init(const EngineConfig& config);

  /**
   * @brief Gracefully shuts down the engine and releases all resources.
   */
  static void Shutdown();

  /**
   * @brief Provides access to the main application window.
   * @return Reference to the main Window.
   */
  static Window& window() { return *internal_window_; }

 private:
  static std::unique_ptr<Window> internal_window_;
};
}  // namespace engine

#endif  // INCLUDE_ENGINE_H_
