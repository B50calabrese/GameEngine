#include <engine/graphics/ecs/text_renderer.h>
#include <engine/util/logger.h>

#include "../common/demo_utils.h"

class MyApp : public demos::common::BaseDemoApp {
 public:
  void OnDemoInit() override {
    LOG_INFO("Initializing Application");
    // "default" font is already loaded by BaseDemoApp
  }

  void OnDemoShutdown() override { LOG_INFO("Shutting down Application"); }

  void OnDemoUpdate(double delta_time_seconds) override {
    engine::graphics::ecs::TextRenderer::Get().DrawText(
        "default", "HELLO WORLD!", glm::vec2(50.0f), /*rotation=*/0.0f,
        /*scale=*/1.0f, glm::vec4(1.0f));

    engine::graphics::ecs::TextRenderer::Get().DrawText(
        "default", "I am rotated!", glm::vec2(100.0f), /*rotation=*/45.0f,
        /*scale=*/1.0f, glm::vec4(1.0f));

    engine::graphics::ecs::TextRenderer::Get().DrawText(
        "default", "I am rotated and scaled!", glm::vec2(200.0f),
        /*rotation=*/45.0f,
        /*scale=*/3.0f, glm::vec4(1.0f));
  }

 private:
};

/**
 * @brief Demo showing off the text rendering systems.
 */
int main(void) {
  engine::EngineConfig engine_config;
  engine_config.asset_path = ENGINE_ASSETS_PATH;
  return demos::common::DemoRunner::Run<MyApp>(engine_config);
}
