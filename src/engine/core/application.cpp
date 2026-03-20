/**
 * @file application.cpp
 * @brief Application class implementation.
 */

#include <engine/core/application.h>
#include <engine/core/engine.h>
#include <engine/core/job_system.h>
#include <engine/core/window.h>
#include <engine/graphics/camera.h>
#include <engine/graphics/particle_system.h>
#include <engine/graphics/render_queue.h>
#include <engine/graphics/renderer.h>
#include <engine/graphics/sprite_render_system.h>
#include <engine/input/action_manager.h>
#include <engine/input/input_manager.h>
#include <engine/physics/physics_system.h>
#include <engine/scene/scene_manager.h>
#include <engine/ui/ui_systems.h>

namespace engine {

static Application* s_instance = nullptr;

Application& Application::Get() { return *s_instance; }

Application::Application() {
  if (s_instance == nullptr) s_instance = this;
}

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
    ActionManager::Get().Update();

    [[maybe_unused]] bool input_handled = SceneManager::Get().DispatchInput();

    graphics::RenderQueue::Default().Clear();

    graphics::Renderer::Get().BeginFrame(*main_camera_);
    graphics::Renderer::Get().Clear();

    SceneManager::Get().UpdateActiveScene(static_cast<float>(delta_time));

    Scene* active_scene = SceneManager::Get().GetActiveScene();
    if (active_scene) {
      ecs::Registry& reg = active_scene->registry();

      // Engine Core Systems
      physics::PhysicsSystem::Update(reg, static_cast<float>(delta_time));

      // Particle Systems
      reg.ForEach<graphics::ParticleEmitterComponent>(
          [dt = static_cast<float>(delta_time)](
              graphics::ParticleEmitterComponent& pec) {
            if (pec.is_active) {
              pec.system.Update(dt);
            }
          });

      // UI Systems
      ui::UISyncSystem::Update(reg);
      ui::UIInputSystem::Update(reg);
      ui::UILayoutSystem::Update(reg, win.width(), win.height());
    }

    SceneManager::Get().RenderActiveScene();
    this->OnUpdate(delta_time);

    // Render ECS-driven graphics
    if (active_scene) {
      graphics::SpriteRenderSystem::Render(active_scene->registry());
      active_scene->registry().ForEach<graphics::ParticleEmitterComponent>(
          [](graphics::ParticleEmitterComponent& pec) { pec.system.Render(); });
    }

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
  Engine::Shutdown();
}

}  // namespace engine
