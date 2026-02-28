#include "application.h"

#include "graphics/camera.h"
#include "graphics/renderer.h"
#include "input_manager.h"
#include "scene_manager.h"
#include "window.h"

namespace engine {

void Application::run() {
  on_init();

  Window& win = window();
  InputManager& input = input_manager();
  main_camera_ = std::make_unique<engine::graphics::Camera>(
      0.0f, (float)win.width(), 0.0f, (float)win.height());

  while (win.is_running()) {
    double delta_time = win.delta_time();  // Get time since last frame

    // Update the input manager's state and poll for events.
    input.update_state();
    win.poll_events();

    // Dispatch the new input to the appropriate scenes.
    bool input_handled = SceneManager::get().dispatch_input();

    // Pre-rendering calls to prepare the renderer prior to drawing anything.
    graphics::Renderer::get().clear();
    graphics::Renderer::get().begin_frame(*main_camera_);

    // Run scene specific update and rendering logic.
    SceneManager::get().update_active_scene((float)delta_time);
    SceneManager::get().render_active_scene();

    // Run application wide update logic.
    this->on_update(delta_time);

    // End the frame rendering and flush the renderer
    graphics::Renderer::get().end_frame();
    win.swap_buffers();
  }

  // 5. Shutdown
  on_shutdown();
}

}  // namespace engine
