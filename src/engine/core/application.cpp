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

Application* Application::instance_ = nullptr;

Application& Application::Get() { return *instance_; }

Application::Application() {
  if (instance_ == nullptr) {
    instance_ = this;
  }
}

void Application::Run() {
  util::ScriptManager::Get().Init();

  InitImGui();
  OnInit();

  Window& win = GetWindow();
  main_camera_ = std::make_unique<engine::graphics::Camera>(
      0.0f, static_cast<float>(win.GetWidth()), 0.0f,
      static_cast<float>(win.GetHeight()));

  while (win.IsRunning()) {
    ProcessFrame();
  }

  OnShutdown();
  ShutdownImGui();
  Engine::Shutdown();
}

void Application::InitImGui() {
  Window& win = GetWindow();
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  ImGui::StyleColorsDark();

  ImGui_ImplGlfw_InitForOpenGL(win.GetNativeHandle(), true);
  ImGui_ImplOpenGL3_Init("#version 330");
}

void Application::ShutdownImGui() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

void Application::ProcessFrame() {
  Window& win = GetWindow();
  double delta_time = win.GetDeltaTime();
  float dt = static_cast<float>(delta_time);

  win.PollEvents();

  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  ActionManager::Get().Update();

  if (InputManager::Get().IsKeyPressed(util::Console::Get().GetToggleKey())) {
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

  if (!util::Console::Get().IsPaused()) {
    UpdateSystems(dt);
    OnUpdate(delta_time);
  }

  RenderSystems();

  util::Console::Get().Render();

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  graphics::utils::RenderQueue::Default().Flush();
  graphics::Renderer::Get().EndFrame();
  win.SwapBuffers();
}

void Application::UpdateSystems(float dt) {
  SceneManager::Get().UpdateActiveScene(dt);
  Scene* active_scene = SceneManager::Get().GetActiveScene();

  if (active_scene) {
    engine::ecs::Registry& reg = active_scene->GetRegistry();
    ecs::systems::ScriptSystem::Init(&reg);

    ecs::systems::ScriptSystem::Update(&reg, dt);
    ecs::systems::PhysicsSystem::Update(&reg, dt);

    reg.ForEach<engine::ecs::components::ParticleEmitter>(
        [dt](engine::ecs::components::ParticleEmitter& pec) {
          if (pec.is_active) {
            pec.system.Update(dt);
          }
        });

    ui::UISyncSystem::Update(reg);
    ui::UIInputSystem::Update(reg);
    ui::UILayoutSystem::Update(reg, GetWindow().GetWidth(),
                               GetWindow().GetHeight());
  }
}

void Application::RenderSystems() {
  SceneManager::Get().RenderActiveScene();
  Scene* active_scene = SceneManager::Get().GetActiveScene();

  if (active_scene) {
    graphics::ecs::SpriteRenderSystem::Render(&active_scene->GetRegistry());
    active_scene->GetRegistry()
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
    ui_render_system.Render(active_scene->GetRegistry(), GetWindow().GetWidth(),
                            GetWindow().GetHeight());
  }
}

}  // namespace engine
