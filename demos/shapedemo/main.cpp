#include <cmath>

#include <glm/gtc/constants.hpp>

#include <engine/graphics/utils/render_queue.h>
#include <engine/util/logger.h>

#include "../../demos/common/demo_utils.h"

using namespace engine::graphics::utils;

class ShapeDemo : public demos::common::BaseDemoApp {
 public:
  void OnDemoInit() override { LOG_INFO("Shape Demo Initialized"); }

  void OnDemoUpdate(double dt) override {
    time_ += static_cast<float>(dt);

    auto& queue = RenderQueue::Default();

    // 1. Quads with various styles
    // Normal Quad
    queue.Submit({.z_order = 0.0f,
                  .position = {50, 50},
                  .size = {100, 100},
                  .color = {1.0f, 0.0f, 0.0f, 1.0f}});

    // Rounded Quad
    queue.Submit({.z_order = 0.0f,
                  .position = {200, 50},
                  .size = {100, 100},
                  .color = {0.0f, 1.0f, 0.0f, 1.0f},
                  .roundness = 0.2f});

    // Outlined Quad
    queue.Submit({.z_order = 0.0f,
                  .position = {350, 50},
                  .size = {100, 100},
                  .color = {0.0f, 0.0f, 1.0f, 1.0f},
                  .thickness = 0.1f});

    // 2. Circles
    // Solid Circle
    queue.Submit({.z_order = 0.0f,
                  .position = {50, 200},
                  .size = {100, 100},
                  .color = {1.0f, 1.0f, 0.0f, 1.0f},
                  .shape_type = ShapeType::kCircle});

    // Outlined Circle
    queue.Submit({.z_order = 0.0f,
                  .position = {200, 200},
                  .size = {100, 100},
                  .color = {1.0f, 0.0f, 1.0f, 1.0f},
                  .shape_type = ShapeType::kCircle,
                  .thickness = 0.1f});

    // Gradient Circle
    queue.Submit({
        .z_order = 0.0f,
        .position = {350, 200},
        .size = {100, 100},
        .color = {0.0f, 1.0f, 1.0f, 1.0f},
        .color2 = {0.0f, 0.0f, 0.5f, 1.0f},
        .shape_type = ShapeType::kCircle,
        .gradient_type = 2  // Radial
    });

    // 3. Triangles
    // Rotating Triangle
    queue.Submit({.z_order = 0.0f,
                  .position = {100, 400},
                  .size = {100, 100},
                  .color = {1.0f, 0.5f, 0.0f, 1.0f},
                  .rotation = time_ * 50.0f,
                  .origin = {0.5f, 0.5f},
                  .shape_type = ShapeType::kTriangle});

    // 4. Lines and Points
    // Line (Solid)
    queue.Submit({.z_order = 0.0f,
                  .position = {500, 400},
                  .size = {700, 500},
                  .color = {1.0f, 1.0f, 1.0f, 1.0f},
                  .shape_type = ShapeType::kLine,
                  .thickness = 5.0f});

    // Dashed Line
    queue.Submit({.z_order = 0.0f,
                  .position = {500, 450},
                  .size = {700, 550},
                  .color = {1.0f, 1.0f, 1.0f, 1.0f},
                  .shape_type = ShapeType::kLine,
                  .thickness = 5.0f,
                  .is_dashed = true});

    // Point
    queue.Submit({.z_order = 0.0f,
                  .position = {600, 400},
                  .color = {1.0f, 1.0f, 1.0f, 1.0f},
                  .shape_type = ShapeType::kPoint,
                  .thickness = 10.0f});

    // 5. Linear Gradient Quad
    queue.Submit({
        .z_order = 0.0f,
        .position = {500, 50},
        .size = {200, 100},
        .color = {1.0f, 1.0f, 1.0f, 1.0f},
        .color2 = {0.0f, 0.0f, 0.0f, 1.0f},
        .gradient_type = 1  // Linear
    });

    // 6. Polygon
    std::vector<glm::vec2> poly = {
        {500, 200}, {550, 150}, {600, 200}, {580, 250}, {520, 250}};
    queue.Submit({.z_order = 0.0f,
                  .color = {0.5f, 0.5f, 1.0f, 1.0f},
                  .shape_type = ShapeType::kPolygon,
                  .polygon_vertices = poly});
  }

 private:
  float time_ = 0.0f;
};

int main(int argc, char** argv) {
  engine::EngineConfig config;
  config.window_title = "Advanced Shapes and Styles Demo";
  config.window_width = 800;
  config.window_height = 600;
  return demos::common::DemoRunner::Run<ShapeDemo>(config);
}
