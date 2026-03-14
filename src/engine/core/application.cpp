/**
 * @file application.cpp
 * @brief Application class implementation.
 */

#include <engine/core/application.h>
#include <engine/core/job_system.h>
#include <engine/core/window.h>
#include <engine/graphics/camera.h>
#include <engine/graphics/render_queue.h>
#include <engine/graphics/renderer.h>
#include <engine/input/action_manager.h>
#include <engine/input/input_manager.h>
#include <engine/scene/scene_manager.h>

namespace engine {

void Application::Run() {
  OnInit();

  Window& win = window();
  InputManager& input = input_manager();
  main_camera_ = std::make_unique<engine::graphics::Camera>(
      0.0f, static_cast<float>(win.width()), 0.0f,
      static_cast<float>(win.height()));

  while (win.IsRunning()) {
    double delta_time = win.delta_time();  // Get time since last frame

    // Update the input manager's state and poll for events.
    input.UpdateState();
    win.PollEvents();
    ActionManager::Get().Update();

    // Dispatch the new input to the appropriate scenes.
    [[maybe_unused]] bool input_handled = SceneManager::Get().DispatchInput();

    // Reset the render queue for the new frame.
    graphics::RenderQueue::Default().Clear();

    // Pre-rendering calls to prepare the renderer prior to drawing anything.
    graphics::Renderer::Get().BeginFrame(*main_camera_);
    graphics::Renderer::Get().Clear();

    // Run scene specific update and rendering logic.
    SceneManager::Get().UpdateActiveScene(static_cast<float>(delta_time));
    SceneManager::Get().RenderActiveScene();

    // Run application wide update logic.
    this->OnUpdate(delta_time);

    // Synchronization point: ensure all background jobs are finished before
    // proceeding to finalize the frame.
    core::JobSystem::Get().Wait();

    // Finalize rendering by flushing the command queue.
    graphics::RenderQueue::Default().Flush();

    // End the frame rendering and flush the renderer
    graphics::Renderer::Get().EndFrame();
    win.SwapBuffers();
  }

  // 5. Shutdown
  OnShutdown();
}

}  // namespace engine
