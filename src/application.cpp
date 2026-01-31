#include "application.h"

#include "graphics/camera.h"
#include "graphics/renderer.h"
#include "input_manager.h"
#include "scene_manager.h"
#include "window.h"

namespace engine {

void Application::Run() {
  OnInit();

  Window& win = window();
  InputManager& input = input_manager();
  main_camera_ = std::make_unique<engine::graphics::Camera>(
      0.0f, (float)win.width(), 0.0f, (float)win.height());

  while (win.IsRunning()) {
    double delta_time = win.delta_time();  // Get time since last frame

    // Update the input manager's state and poll for events.
    input.UpdateState();
    win.PollEvents();

    // Dispatch the new input to the appropriate scenes.
    bool input_handled = SceneManager::Get().DispatchInput();

    // Pre-rendering calls to prepare the renderer prior to drawing anything.
    graphics::Renderer::Get().Clear();
    graphics::Renderer::Get().BeginFrame(*main_camera_);

    // Run scene specific update and rendering logic.
    SceneManager::Get().UpdateActiveScene((float)delta_time);
    SceneManager::Get().RenderActiveScene();

    // Run application wide update logic.
    this->OnUpdate(delta_time);

    // End the frame rendering and flush the renderer
    graphics::Renderer::Get().EndFrame();
    win.SwapBuffers();
  }

  // 5. Shutdown
  OnShutdown();
}

}  // namespace engine
