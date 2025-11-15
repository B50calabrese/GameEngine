#ifndef ENGINE_H
#define ENGINE_H

#include <memory>
#include <string>

#include "window.h"

namespace engine {

struct EngineConfig {
  int window_height = 720;
  int window_width = 1280;
  std::string window_title = "Title";
};

class Engine {
 public:
  // Initializes the engine itself, including OpenGL init, hints, etc.
  static void Init(const EngineConfig& config);

  // Gracefully shutsdown the engine.
  static void Shutdown();

  // Access to the Window instance that the Engine creates on Init.
  static Window& GetWindow() { return *internal_window; }

 private:
  static std::unique_ptr<Window> internal_window;
};
}  // namespace engine

#endif  // ENGINE_H