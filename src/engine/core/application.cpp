/**
 * @file application.cpp
 * @brief Application class implementation.
 */

#include <engine/core/application.h>
#include <engine/core/engine.h>
#include <engine/core/job_system.h>
#include <engine/core/window.h>
#include <engine/ecs/script_system.h>
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
#include <engine/util/console.h>
#include <engine/util/scripting/script_manager.h>

namespace engine {

static Application* g_application_instance = nullptr;

Application& Application::Get() { return *g_application_instance; }

Application::Application() {
  if (g_application_instance == nullptr) {
    g_application_instance = this;
  }
}

void Application::Run() {
  util::ScriptManager::Get().Init();
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

    if (input.IsKeyPressed(util::Console::Get().GetToggleKey())) {
      util::Console::Get().Toggle();
    }

    util::Console::Get().Update();

    [[maybe_unused]] bool input_handled = false;
    if (util::Console::Get().IsVisible()) {
      input_handled = true;
    } else {
      input_handled = SceneManager::Get().DispatchInput();
    }

    graphics::RenderQueue::Default().Clear();

    graphics::Renderer::Get().BeginFrame(*main_camera_);
    graphics::Renderer::Get().Clear();

    SceneManager::Get().UpdateActiveScene(static_cast<float>(delta_time));

    Scene* active_scene = SceneManager::Get().GetActiveScene();
    if (active_scene) {
      ecs::Registry& reg = active_scene->registry();

      // Initialize ScriptSystem for the current registry if needed
      ecs::ScriptSystem::Init(&reg);

      if (!util::Console::Get().IsPaused()) {
        // Scripting System (Update logic before physics)
        ecs::ScriptSystem::Update(&reg, static_cast<float>(delta_time));

        // Engine Core Systems
        physics::PhysicsSystem::Update(&reg, static_cast<float>(delta_time));

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

        this->OnUpdate(delta_time);
      }
    }

    SceneManager::Get().RenderActiveScene();

    // Render ECS-driven graphics
    if (active_scene) {
      graphics::SpriteRenderSystem::Render(&active_scene->registry());
      active_scene->registry().ForEach<graphics::ParticleEmitterComponent>(
          [](graphics::ParticleEmitterComponent& pec) {
            pec.system.Render(pec.z_index);
          });
    }

    core::JobSystem::Get().Wait();

    graphics::RenderQueue::Default().Flush();
    graphics::Renderer::Get().Flush();

    if (active_scene) {
      static ui::UIRenderSystem ui_render_system;
      ui_render_system.Render(active_scene->registry(), win.width(),
                              win.height());
    }

    util::Console::Get().Render();

    graphics::Renderer::Get().EndFrame();
    win.SwapBuffers();
  }
  OnShutdown();
  Engine::Shutdown();
}

}  // namespace engine
