#ifndef APPLICATION_H
#define APPLICATION_H

#include "engine.h"
#include "input_manager.h"
#include "window.h"

namespace engine {

// Represents the client application that runs on top of the engine. The client
// is expected to subclass `Application` and override the lifecycle methods.
class Application {
 public:
  // Drives the main game loop. This method will continuously call `OnUpdate`
  // until the application is terminated.
  void Run();

  // Called once when the application starts. This is the ideal place for
  // one-time initialization code, such as loading assets.
  virtual void OnInit() = 0;

  // Called once when the application is about to close. Use this for cleanup
  // code.
  virtual void OnShutdown() = 0;

  // Called every frame. Game logic and rendering should be handled here.
  //
  // @param deltaTimeSeconds The time elapsed since the last frame, in seconds.
  //        This value should be used for frame-rate independent physics and
  //        animations.
  virtual void OnUpdate(double deltaTimeSeconds) = 0;

  // Virtual destructor is crucial for proper inheritance cleanup.
  virtual ~Application() = default;

 protected:
  // Provides access to the main application window.
  Window& GetWindow() { return Engine::GetWindow(); }

  // Provides access to the input manager for handling keyboard and mouse
  // events.
  InputManager& GetInputManager() { return InputManager::Get(); }
};

}  // namespace engine

#endif  // APPLICATION_H