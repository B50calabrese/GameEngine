#include <vector>

// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

#include <engine/ecs/components/graphics_components.h>
#include <engine/ecs/components/transform.h>
#include <engine/ecs/registry.h>
#include <engine/graphics/ecs/lighting_system.h>
#include <engine/graphics/lighting_effect.h>
#include <engine/graphics/post_processor.h>
#include <engine/graphics/renderer.h>
#include <engine/input/action_manager.h>
#include <engine/input/input_manager.h>
#include <engine/util/logger.h>

#include "../common/demo_utils.h"

class LightingDemo : public demos::common::BaseDemoApp {
 public:
  void OnDemoInit() override {
    // 1. Setup Post-Processing with Lighting
    auto lighting = std::make_unique<engine::graphics::LightingEffect>();
    lighting_ptr_ = lighting.get();

    // Configure initial lighting
    lighting_ptr_->OnResize(800, 600);
    lighting_ptr_->SetAmbientLight({0.1f, 0.1f, 0.2f}, 0.3f);
    lighting_ptr_->SetShadowType(engine::graphics::ShadowType::Raymarch);

    engine::graphics::PostProcessManager::Get().AddEffect(std::move(lighting));

    // 2. Setup ECS Entities
    // A moving light
    light_entity_ = registry_.CreateEntity();
    engine::ecs::components::Transform light_transform;
    light_transform.position = glm::vec2(400.0f, 300.0f);
    registry_.AddComponent<engine::ecs::components::Transform>(light_entity_,
                                                               light_transform);

    engine::ecs::components::Light light_comp;
    light_comp.color = glm::vec3(1.0f, 0.8f, 0.4f);
    light_comp.intensity = 1.5f;
    light_comp.radius = 250.0f;
    registry_.AddComponent<engine::ecs::components::Light>(light_entity_,
                                                           light_comp);

    // Some static occluders
    auto occluder1 = registry_.CreateEntity();
    engine::ecs::components::Transform occ1_transform;
    occ1_transform.position = glm::vec2(200.0f, 200.0f);
    registry_.AddComponent<engine::ecs::components::Transform>(occluder1,
                                                               occ1_transform);
    engine::ecs::components::Occluder occ1_comp;
    occ1_comp.size = glm::vec2(50.0f, 50.0f);
    registry_.AddComponent<engine::ecs::components::Occluder>(occluder1,
                                                              occ1_comp);

    auto occluder2 = registry_.CreateEntity();
    engine::ecs::components::Transform occ2_transform;
    occ2_transform.position = glm::vec2(600.0f, 400.0f);
    registry_.AddComponent<engine::ecs::components::Transform>(occluder2,
                                                               occ2_transform);
    engine::ecs::components::Occluder occ2_comp;
    occ2_comp.size = glm::vec2(80.0f, 30.0f);
    registry_.AddComponent<engine::ecs::components::Occluder>(occluder2,
                                                              occ2_comp);

    // Spotlight
    spotlight_entity_ = registry_.CreateEntity();
    engine::ecs::components::Transform spot_transform;
    spot_transform.position = glm::vec2(100.0f, 500.0f);
    registry_.AddComponent<engine::ecs::components::Transform>(
        spotlight_entity_, spot_transform);
    engine::ecs::components::Light spot_comp;
    spot_comp.color = glm::vec3(0.2f, 1.0f, 0.2f);  // Green
    spot_comp.intensity = 2.0f;
    spot_comp.radius = 400.0f;
    spot_comp.angle = 45.0f;
    spot_comp.dir_vector = glm::vec2(1.0f, -1.0f);
    registry_.AddComponent<engine::ecs::components::Light>(spotlight_entity_,
                                                           spot_comp);

    // Directional engine::graphics::Light (Ambient-ish but directional)
    dir_light_entity_ = registry_.CreateEntity();
    engine::ecs::components::Light dir_comp;
    dir_comp.color = glm::vec3(0.2f, 0.2f, 0.5f);
    dir_comp.intensity = 0.5f;
    dir_comp.is_directional = true;
    dir_comp.dir_vector = glm::vec2(0.0f, -1.0f);
    registry_.AddComponent<engine::ecs::components::Light>(dir_light_entity_,
                                                           dir_comp);

    // 3. Register Controls
    engine::ActionManager::Get().BindAction("ToggleShadows",
                                            engine::KeyCode::kT);
    engine::ActionManager::Get().BindAction("ToggleMode", engine::KeyCode::kM);

    LOG_INFO("2D Lighting Demo Initialized");
    LOG_INFO("Move mouse to move the light");
    LOG_INFO(
        "Press 'T' to toggle shadows (by clearing occluders in this demo)");
    LOG_INFO(
        "Press 'M' to toggle between Additive and Multiplicative blending");
  }

  void OnDemoShutdown() override { LOG_INFO("2D Lighting Demo Shutting Down"); }

  void OnDemoUpdate(double delta_time) override {
    // Update light position to mouse (mocking movement for headless capture)
    auto& transform =
        registry_.GetComponent<engine::ecs::components::Transform>(
            light_entity_);
    // In a real app this would be: transform.position =
    // engine::InputManager::Get().mouse_screen_pos(); For headless capture
    // we'll move it in a circle
    float time = static_cast<float>(glfwGetTime());
    transform.position =
        glm::vec2(400.0f + cos(time) * 100.0f, 300.0f + sin(time) * 100.0f);

    if (engine::ActionManager::Get().IsStarted("ToggleShadows")) {
      shadows_enabled_ = !shadows_enabled_;
      if (!shadows_enabled_) {
        lighting_ptr_->ClearOccluders();
      }
    }

    if (engine::ActionManager::Get().IsStarted("ToggleMode")) {
      multiplicative_ = !multiplicative_;
      lighting_ptr_->SetUseMultiplicativeBlending(multiplicative_);
    }

    // Cycle shadow type
    if (input_manager().IsKeyPressed(engine::KeyCode::kB)) {
      shadow_blur_ = (shadow_blur_ == 0.0f) ? 0.05f : 0.0f;
      lighting_ptr_->SetShadowBlur(shadow_blur_);
    }

    // Sync ECS to Lighting Effect
    if (shadows_enabled_) {
      engine::graphics::ecs::UpdateLightingSystem(&registry_, lighting_ptr_);
    } else {
      engine::graphics::ecs::UpdateLightingSystem(&registry_, lighting_ptr_);
      lighting_ptr_->ClearOccluders();
    }

    // Render engine::Scene
    // Background
    engine::graphics::Renderer::Get().DrawRect(0, 0, 800, 600, 0.2f, 0.2f,
                                               0.2f);

    // Some "floor" tiles
    for (int x = 0; x < 800; x += 100) {
      for (int y = 0; y < 600; y += 100) {
        float c = ((x + y) / 100) % 2 == 0 ? 0.3f : 0.4f;
        engine::graphics::Renderer::Get().DrawRect(x, y, 100, 100, c, c, c);
      }
    }

    // Draw the occluders so we can see them
    auto view = registry_.GetView<engine::ecs::components::Transform,
                                  engine::ecs::components::Occluder>();
    for (auto entity : view) {
      auto& t =
          registry_.GetComponent<engine::ecs::components::Transform>(entity);
      auto& o =
          registry_.GetComponent<engine::ecs::components::Occluder>(entity);
      engine::graphics::Renderer::Get().DrawRect(
          t.position.x - o.size.x / 2, t.position.y - o.size.y / 2, o.size.x,
          o.size.y, 0.1f, 0.1f, 0.1f);
    }
  }

 private:
  engine::ecs::Registry registry_;
  engine::ecs::EntityID light_entity_;
  engine::ecs::EntityID spotlight_entity_;
  engine::ecs::EntityID dir_light_entity_;
  engine::graphics::LightingEffect* lighting_ptr_ = nullptr;
  bool shadows_enabled_ = true;
  bool multiplicative_ = true;
  float shadow_blur_ = 0.0f;
};

int main() {
  engine::EngineConfig config;
  config.window_width = 800;
  config.window_height = 600;
  return demos::common::DemoRunner::Run<LightingDemo>(config);
}
