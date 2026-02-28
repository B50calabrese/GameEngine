#include <iostream>

#include "application.h"
#include "engine.h"
#include "graphics/text_renderer.h"

class MyApp : public engine::Application {
 public:
  void on_init() override {
    std::cout << "Initializing Application" << std::endl;
    engine::graphics::TextRenderer::get().init();
    engine::graphics::TextRenderer::get().load_font("arial", "arial.ttf", 16);
  }

  void on_shutdown() override {
    std::cout << "Shutting down Application" << std::endl;
  }

  void on_update(double delta_time_seconds) override {
    engine::graphics::TextRenderer::get().draw_text(
        "arial", "HELLO WORLD!", glm::vec2(50.0f), /*rotation=*/0.0f,
        /*scale=*/1.0f, glm::vec4(1.0f));

    engine::graphics::TextRenderer::get().draw_text(
        "arial", "I am rotated!", glm::vec2(100.0f), /*rotation=*/45.0f,
        /*scale=*/1.0f, glm::vec4(1.0f));

    engine::graphics::TextRenderer::get().draw_text(
        "arial", "I am rotated and scaled!", glm::vec2(200.0f),
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
  engine::Engine::init(engine_config);
  MyApp my_app;
  my_app.run();
  return 0;
}
