#pragma once

#include <memory>
#include <string>

#include "window.h"

namespace engine {

// EngineConfig provides a way to configure the game engine at startup.
struct EngineConfig {
  // The height of the main application window.
  int window_height = 720;
  // The width of the main application window.
  int window_width = 1280;
  // The title of the main application window.
  std::string window_title = "Title";
};

// The Engine class is the main entry point for the game engine.
// It is responsible for initializing the engine, running the main loop, and
// shutting down the engine.
class Engine {
 public:
  // Initializes the engine itself, including OpenGL init, hints, etc.
  // @param config The configuration for the engine.
  static void Init(const EngineConfig& config);

  // Gracefully shutsdown the engine.
  static void Shutdown();

  // Access to the Window instance that the Engine creates on Init.
  // @return A reference to the main application window.
  static Window& GetWindow() { return *window_; }

 private:
  // The main application window.
  static std::unique_ptr<Window> window_;
};

}  // namespace engine
