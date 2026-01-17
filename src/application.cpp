#include "application.h"

#include "graphics/renderer.h"
#include "input_manager.h"
#include "scene_manager.h"
#include "window.h"

namespace engine {

void Application::Run() {
  OnInit();

  Window& window = GetWindow();
  InputManager& input = GetInputManager();

  while (window.IsRunning()) {
    double delta_time = window.GetDeltaTime();  // Get time since last frame

    // Update the input manager's state and poll for events.
    input.UpdateState();
    window.PollEvents();

    // Dispatch the new input to the appropriate scenes.
    bool inputHandled = SceneManager::Get().DispatchInput();

    // Pre-rendering calls to prepare the renderer prior to drawing anything.
    graphics::Renderer::Get().Clear();
    graphics::Renderer::Get().BeginFrame();

    // Run scene specific update and rendering logic.
    SceneManager::Get().UpdateActiveScene(delta_time);
    SceneManager::Get().RenderActiveScene();

    // Run application wide update logic.
    this->OnUpdate(delta_time);

    // End the frame rendering and flush the renderer
    graphics::Renderer::Get().EndFrame();
    window.SwapBuffers();
  }

  // 5. Shutdown
  OnShutdown();
}

}  // namespace engine