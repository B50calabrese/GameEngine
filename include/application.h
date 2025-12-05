#ifndef APPLICATION_H
#define APPLICATION_H

#include "engine.h"
#include "input_manager.h"
#include "window.h"

namespace engine {

class Application {
 public:
  // Core engine loop driver
  void Run();

  // Lifecycle methods that the client will override
  virtual void OnInit() = 0;
  virtual void OnShutdown() = 0;
  virtual void OnUpdate(double deltaTimeSeconds) = 0;

  // Virtual destructor is crucial for proper inheritance cleanup
  virtual ~Application() = default;

 protected:
  // Protected access to engine managers for the derived class
  Window& GetWindow() { return Engine::GetWindow(); }
  InputManager& GetInputManager() { return InputManager::Get(); }
};

}  // namespace engine

#endif  // APPLICATION_H