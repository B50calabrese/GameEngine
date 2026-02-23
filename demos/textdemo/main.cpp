#include <iostream>

#include "application.h"
#include "engine.h"
#include "graphics/text_renderer.h"

class MyApp : public engine::Application {
 public:
  void OnInit() override {
    std::cout << "Initializing Application" << std::endl;
    engine::graphics::TextRenderer::Get().Init();
    engine::graphics::TextRenderer::Get().LoadFont("arial", "arial.ttf", 16);
  }

  void OnShutdown() override {
    std::cout << "Shutting down Application" << std::endl;
  }

  void OnUpdate(double deltaTimeSeconds) override {
    engine::graphics::TextRenderer::Get().DrawText(
        "arial", "HELLO WORLD!", glm::vec2(50.0f), /*rotation=*/0.0f,
        /*scale=*/1.0f, glm::vec4(1.0f));

    engine::graphics::TextRenderer::Get().DrawText(
        "arial", "I am rotated!", glm::vec2(100.0f), /*rotation=*/45.0f,
        /*scale=*/1.0f, glm::vec4(1.0f));

    engine::graphics::TextRenderer::Get().DrawText(
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
  engine::Engine::Init(engine_config);
  MyApp my_app;
  my_app.Run();
  return 0;
}
