/**
 * @file application.cpp
 * @brief Application class implementation.
 */

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <engine/core/application.h>
#include <engine/core/engine.h>
#include <engine/core/job_system.h>
#include <engine/core/window.h>
#include <engine/ecs/components/particle_emitter.h>
#include <engine/ecs/systems/ai_system.h>
#include <engine/ecs/systems/camera_system.h>
#include <engine/ecs/systems/physics_system.h>
#include <engine/ecs/systems/script_system.h>
#include <engine/graphics/camera.h>
#include <engine/graphics/ecs/sprite_render_system.h>
#include <engine/graphics/renderer.h>
#include <engine/graphics/utils/particle_system.h>
#include <engine/graphics/utils/render_queue.h>
#include <engine/input/action_manager.h>
#include <engine/input/input_manager.h>
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

  Window& win = window();

  // Initialize ImGui
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  (void)io;
  io.ConfigFlags |=
      ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls

  ImGui::StyleColorsDark();

  ImGui_ImplGlfw_InitForOpenGL(win.native_handle(), true);
  ImGui_ImplOpenGL3_Init("#version 330");

  OnInit();
  InputManager& input = input_manager();
  main_camera_ = std::make_unique<engine::graphics::Camera>(
      0.0f, static_cast<float>(win.width()), 0.0f,
      static_cast<float>(win.height()));

  while (win.IsRunning()) {
    double delta_time = win.delta_time();

    win.PollEvents();

    // Start ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

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

    graphics::utils::RenderQueue::Default().Clear();

    graphics::Renderer::Get().BeginFrame(*main_camera_);
    graphics::Renderer::Get().Clear();

    SceneManager::Get().UpdateActiveScene(static_cast<float>(delta_time));

    Scene* active_scene = SceneManager::Get().GetActiveScene();
    if (active_scene) {
      engine::ecs::Registry& reg = active_scene->registry();

      // Initialize ScriptSystem for the current registry if needed
      ecs::systems::ScriptSystem::Init(&reg);

      if (!util::Console::Get().IsPaused()) {
        // Scripting System (Update logic before physics)
        ecs::systems::ScriptSystem::Update(&reg,
                                           static_cast<float>(delta_time));

        // AI System (Animations, Lifetime, Pathing)
        ecs::systems::AISystem::Update(&reg, static_cast<float>(delta_time));

        // Engine Core Systems
        ecs::systems::PhysicsSystem::Update(&reg,
                                            static_cast<float>(delta_time));

        // Sync Camera
        ecs::systems::CameraSystem::Update(&reg);

        // Particle Systems
        reg.ForEach<engine::ecs::components::ParticleEmitter>(
            [dt = static_cast<float>(delta_time)](
                engine::ecs::components::ParticleEmitter& pec) {
              if (pec.is_active) {
                pec.system.Update(dt);
              }
            });

        // UI Systems
        ui::UISyncSystem::Update(reg);
        ui::UIInputSystem::Update(reg);
        ui::UILayoutSystem::Update(reg, win.width(), win.height());
      }
    }

    if (!util::Console::Get().IsPaused()) {
      this->OnUpdate(delta_time);
    }

    SceneManager::Get().RenderActiveScene();

    // Render ECS-driven graphics
    if (active_scene) {
      graphics::ecs::SpriteRenderSystem::Render(&active_scene->registry());
      active_scene->registry()
          .ForEach<engine::ecs::components::ParticleEmitter>(
              [](engine::ecs::components::ParticleEmitter& pec) {
                pec.system.Render(pec.z_index);
              });
    }

    core::JobSystem::Get().Wait();

    graphics::utils::RenderQueue::Default().Flush();
    graphics::Renderer::Get().Flush();

    if (active_scene) {
      static ui::UIRenderSystem ui_render_system;
      ui_render_system.Render(active_scene->registry(), win.width(),
                              win.height());
    }

    util::Console::Get().Render();

    // Render ImGui
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    graphics::utils::RenderQueue::Default().Flush();
    graphics::Renderer::Get().EndFrame();

    SceneManager::Get().ProcessPendingChanges();

    win.SwapBuffers();
  }
  OnShutdown();

  // Cleanup ImGui
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  Engine::Shutdown();
}

}  // namespace engine
