#include "application.h"

#include "graphics/renderer.h"
#include "input_manager.h"
#include "window.h"

namespace engine {

void Application::Run() {
  OnInit();

  Window& window = GetWindow();
  InputManager& input = GetInputManager();

  while (window.IsRunning()) {
    double delta_time = window.GetDeltaTime();  // Get time since last frame
    // 1. INPUT STATE TRANSITION (Engine handles this)
    input.UpdateState();

    // 2. PLATFORM EVENT POLLING (Engine handles this)
    window.PollEvents();

    // 3. SCENE/GAME LOGIC UPDATE (Client's responsibility, called
    this->OnUpdate(delta_time - window.GetDeltaTime());

    // 4. RENDERING
    graphics::Renderer::Get().Clear();
    window.SwapBuffers();
  }

  // 5. Shutdown
  OnShutdown();
}

}  // namespace engine