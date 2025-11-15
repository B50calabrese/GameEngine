#include <iostream>

#include "engine.h"
#include "input_manager.h"
#include "window.h"

/**
 * Simple demo to show off creating a window, accessing the input manager, and
 * polling for events.
 */

int main(void) {
  engine::EngineConfig engine_config;
  engine::Engine::Init(engine_config);
  engine::Window& window = engine::Engine::GetWindow();
  engine::InputManager& input_manager = engine::InputManager::Get();
  while (true) {
    window.PollEvents();

    if (input_manager.IsKeyPressed(engine::KeyCode::kW)) {
      std::cout << "W was pressed\n";
    }

    if (input_manager.IsKeyDown(engine::KeyCode::kS)) {
      std::cout << "S is down\n";
    }

    if (input_manager.IsKeyReleased(engine::KeyCode::kA)) {
      std::cout << "A was released\n";
    }
  }
  return 0;
}
