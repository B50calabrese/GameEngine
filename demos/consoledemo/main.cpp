#include <engine/util/console.h>
#include <engine/util/logger.h>

#include "../common/demo_utils.h"

class ConsoleDemo : public demos::common::BaseDemoApp {
 public:
  void OnDemoInit() override {
    LOG_INFO("Console Demo Initialized");

    engine::util::Console::Get().RegisterCommand(
        "set_gold", [](const std::vector<std::string>& args) {
          if (args.empty()) {
            engine::util::Console::Get().Log("Usage: set_gold <amount>");
            return;
          }
          int amount = std::stoi(args[0]);
          engine::util::Console::Get().Log("Gold set to " + std::to_string(amount));
        });

    engine::util::Console::Get().RegisterCommand(
        "spawn_enemy", [](const std::vector<std::string>& args) {
          std::string type = args.empty() ? "grunt" : args[0];
          engine::util::Console::Get().Log("Spawned enemy of type: " + type);
        });
  }

  void OnDemoShutdown() override { LOG_INFO("Console Demo Shutdown"); }

  void OnDemoUpdate(double dt) override {
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
  engine::EngineConfig config;
  config.window_title = "Console Demo";
  config.window_width = 800;
  config.window_height = 600;
  config.asset_path = ENGINE_ASSETS_PATH;

  return demos::common::DemoRunner::Run<ConsoleDemo>(config);
}
