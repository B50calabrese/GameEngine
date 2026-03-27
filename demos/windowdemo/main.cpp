#include <engine/util/logger.h>

#include "../common/demo_utils.h"

class WindowDemoApp : public demos::common::BaseDemoApp {
 public:
  void OnDemoUpdate(double dt) override {
    if (input_manager().IsKeyPressed(engine::KeyCode::kW)) {
      LOG_INFO("W was pressed");
    }

    if (input_manager().IsKeyDown(engine::KeyCode::kS)) {
      LOG_INFO("S is down");
    }

    if (input_manager().IsKeyReleased(engine::KeyCode::kA)) {
      LOG_INFO("A was released");
    }
  }
};

/**
 * @brief Simple demo to show off creating a window, accessing the input
 * manager, and polling for events.
 */
int main(void) {
  engine::EngineConfig config;
  return demos::common::DemoRunner::Run<WindowDemoApp>(config);
}
