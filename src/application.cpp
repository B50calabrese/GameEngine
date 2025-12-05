#include "application.h"

#include "input_manager.h"
#include "window.h"

namespace engine {

void Application::Run() {
  OnInit();

  Window& window = GetWindow();
  InputManager& input = GetInputManager();
  // GameContext& context = GetContext(); // Assuming context access

  while (window.IsRunning()) {
    double delta_time = window.GetDeltaTime();  // Get time since last frame
    // 1. INPUT STATE TRANSITION (Engine handles this)
    input.UpdateState();

    // 2. PLATFORM EVENT POLLING (Engine handles this)
    window.PollEvents();

    // 3. SCENE/GAME LOGIC UPDATE (Client's responsibility, called
    this->OnUpdate(delta_time - window.GetDeltaTime());

    // 4. RENDERING
    // Renderer::Get().Clear();
    // context.GetScreenManager().Render();
    // window.SwapBuffers();
  }

  // 5. Shutdown
  OnShutdown();
}

}  // namespace engine