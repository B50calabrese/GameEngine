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
#include <engine/ui/ui_systems.h>

namespace engine {

void Application::Run() {
  OnInit();

  Window& win = window();
  InputManager& input = input_manager();
  main_camera_ = std::make_unique<engine::graphics::Camera>(
      0.0f, static_cast<float>(win.width()), 0.0f,
      static_cast<float>(win.height()));

  while (win.IsRunning()) {
    double delta_time = win.delta_time();

    win.PollEvents();
    input.UpdateState();
    ActionManager::Get().Update();

    [[maybe_unused]] bool input_handled = SceneManager::Get().DispatchInput();

    graphics::RenderQueue::Default().Clear();

    graphics::Renderer::Get().BeginFrame(*main_camera_);
    graphics::Renderer::Get().Clear();

    SceneManager::Get().UpdateActiveScene(static_cast<float>(delta_time));

    Scene* active_scene = SceneManager::Get().GetActiveScene();
    if (active_scene) {
      ecs::Registry& reg = active_scene->registry();
      ui::UISyncSystem::Update(reg);
      ui::UIInputSystem::Update(reg);
      ui::UILayoutSystem::Update(reg, win.width(), win.height());
    }

    SceneManager::Get().RenderActiveScene();
    this->OnUpdate(delta_time);

    core::JobSystem::Get().Wait();

    graphics::RenderQueue::Default().Flush();
    graphics::Renderer::Get().Flush();

    if (active_scene) {
      static ui::UIRenderSystem ui_render_system;
      ui_render_system.Render(active_scene->registry(), win.width(),
                              win.height());
    }

    graphics::Renderer::Get().EndFrame();
    win.SwapBuffers();
  }
  OnShutdown();
}

}  // namespace engine
