#ifndef ENGINE_H
#define ENGINE_H

#include <memory>
#include <string>

#include "window.h"

namespace engine {

// Holds the configuration for the engine's initialization.
struct EngineConfig {
  // The height of the main application window.
  int window_height = 720;
  // The width of the main application window.
  int window_width = 1280;
  // The title of the main application window.
  std::string window_title = "Title";
  // The default path for assets.
  std::string asset_path = "";
};

// The static `Engine` class provides the core functionality of the game engine,
// including initialization, shutdown, and access to engine-managed systems.
class Engine {
 public:
  // Initializes the engine, including the window, graphics context, and other
  // core systems. This must be called before any other engine functions.
  //
  // @param config The `EngineConfig` struct containing initialization
  // settings.
  static void Init(const EngineConfig& config);

  // Gracefully shuts down the engine and releases all resources.
  static void Shutdown();

  // Provides access to the main application window.
  static Window& GetWindow() { return *internal_window; }

 private:
  static std::unique_ptr<Window> internal_window;
};
}  // namespace engine

#endif  // ENGINE_H