#include <iostream>

#include <engine/core/application.h>
#include <engine/core/engine.h>
#include <engine/graphics/text_renderer.h>
#include <engine/util/console.h>
#include <engine/util/logger.h>

using namespace engine;

class ConsoleDemo : public Application {
 public:
  void OnInit() override {
    engine::graphics::TextRenderer::Get().Init();
    engine::graphics::TextRenderer::Get().LoadFont("default", "arial.ttf", 24);
    LOG_INFO("Console Demo Initialized");

    util::Console::Get().RegisterCommand(
        "set_gold", [](const std::vector<std::string>& args) {
          if (args.empty()) {
            util::Console::Get().Log("Usage: set_gold <amount>");
            return;
          }
          int amount = std::stoi(args[0]);
          util::Console::Get().Log("Gold set to " + std::to_string(amount));
        });

    util::Console::Get().RegisterCommand(
        "spawn_enemy", [](const std::vector<std::string>& args) {
          std::string type = args.empty() ? "grunt" : args[0];
          util::Console::Get().Log("Spawned enemy of type: " + type);
        });
  }

  void OnShutdown() override { LOG_INFO("Console Demo Shutdown"); }

  void OnUpdate(double dt) override {
    // This is skipped when console is paused
    static float timer = 0.0f;
    timer += (float)dt;
    if (timer > 1.0f) {
      // LOG_INFO("Game Logic Running...");
      timer = 0.0f;
    }
  }
};

int main() {
  EngineConfig config;
  config.window_title = "Console Demo";
  config.window_width = 800;
  config.window_height = 600;
  config.asset_path = ENGINE_ASSETS_PATH;

  Engine::Init(config);

  ConsoleDemo app;
  app.Run();

  return 0;
}
