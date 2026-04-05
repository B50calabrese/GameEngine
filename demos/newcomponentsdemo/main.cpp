#include <cmath>
#include <vector>

#include <glm/gtc/constants.hpp>

#include <engine/ecs/components/circle.h>
#include <engine/ecs/components/line.h>
#include <engine/ecs/components/point.h>
#include <engine/ecs/components/polygon.h>
#include <engine/ecs/components/quad.h>
#include <engine/ecs/components/triangle.h>
#include <engine/ecs/components/transform.h>
#include <engine/ecs/components/sprite.h>
#include <engine/ecs/components/animation.h>
#include <engine/ecs/components/lifetime.h>
#include <engine/ecs/components/waypoint_path.h>
#include <engine/ecs/components/camera_component.h>
#include <engine/graphics/utils/render_queue.h>
#include <engine/util/logger.h>
#include <engine/scene/scene_manager.h>

#include "../../demos/common/demo_utils.h"

using namespace engine::ecs::components;

class NewComponentsDemo : public demos::common::BaseDemoApp {
 public:
  void OnDemoInit() override {
    LOG_INFO("New Components Demo Initialized");

    auto* scene = engine::SceneManager::Get().GetActiveScene();
    if (!scene) return;
    auto& reg = scene->registry();

    // 1. Geometry Entities
    // Circle
    auto circle_ent = reg.CreateEntity();
    reg.AddComponent<engine::ecs::components::Transform>(circle_ent, engine::ecs::components::Transform{{100, 100}});
    reg.AddComponent<engine::ecs::components::Circle>(circle_ent, engine::ecs::components::Circle{.radius = 50.0f, .color = {1, 0, 0, 1}, .thickness = 5.0f});

    // Triangle
    auto tri_ent = reg.CreateEntity();
    reg.AddComponent<engine::ecs::components::Transform>(tri_ent, engine::ecs::components::Transform{{250, 100}});
    reg.AddComponent<engine::ecs::components::Triangle>(tri_ent, engine::ecs::components::Triangle{.size = {80, 80}, .color = {0, 1, 0, 1}});

    // Line
    auto line_ent = reg.CreateEntity();
    reg.AddComponent<engine::ecs::components::Transform>(line_ent, engine::ecs::components::Transform{{400, 100}});
    reg.AddComponent<engine::ecs::components::Line>(line_ent, engine::ecs::components::Line{.dest = {100, 50}, .color = {0, 0, 1, 1}, .thickness = 3.0f, .is_dashed = true});

    // Point
    auto point_ent = reg.CreateEntity();
    reg.AddComponent<engine::ecs::components::Transform>(point_ent, engine::ecs::components::Transform{{550, 100}});
    reg.AddComponent<engine::ecs::components::Point>(point_ent, engine::ecs::components::Point{.color = {1, 1, 0, 1}, .size = 10.0f});

    // Polygon
    auto poly_ent = reg.CreateEntity();
    reg.AddComponent<engine::ecs::components::Transform>(poly_ent, engine::ecs::components::Transform{{700, 100}});
    reg.AddComponent<engine::ecs::components::Polygon>(poly_ent, engine::ecs::components::Polygon{.vertices = {{0, 0}, {30, -20}, {60, 0}, {45, 40}, {15, 40}}, .color = {1, 0, 1, 1}});

    // 2. Lifetime Entity
    auto life_ent = reg.CreateEntity();
    reg.AddComponent<engine::ecs::components::Transform>(life_ent, engine::ecs::components::Transform{{100, 300}});
    reg.AddComponent<engine::ecs::components::Circle>(life_ent, engine::ecs::components::Circle{.radius = 30.0f, .color = {1, 1, 1, 1}});
    reg.AddComponent<engine::ecs::components::Lifetime>(life_ent, engine::ecs::components::Lifetime{5.0f}); // Will disappear after 5 seconds

    // 3. Waypoint Path Entity
    auto path_ent = reg.CreateEntity();
    reg.AddComponent<engine::ecs::components::Transform>(path_ent, engine::ecs::components::Transform{{400, 300}});
    reg.AddComponent<engine::ecs::components::Quad>(path_ent, engine::ecs::components::Quad{.color = {0.5f, 0.5f, 0.5f, 1}});
    reg.AddComponent<engine::ecs::components::WaypointPath>(path_ent, engine::ecs::components::WaypointPath{
      .points = {{400, 300}, {600, 300}, {600, 500}, {400, 500}},
      .speed = 150.0f,
      .loop = true
    });

    // 4. Camera Follow Entity (Active Camera)
    auto cam_target = reg.CreateEntity();
    reg.AddComponent<engine::ecs::components::Transform>(cam_target, engine::ecs::components::Transform{{400, 300}});
  }

  void OnDemoUpdate(double dt) override {
  }
};

int main(int argc, char** argv) {
  engine::EngineConfig config;
  config.window_title = "New ECS Components Demo";
  config.window_width = 800;
  config.window_height = 600;
  return demos::common::DemoRunner::Run<NewComponentsDemo>(config);
}
