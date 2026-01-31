#include <iostream>

#include "engine.h"
#include "input_manager.h"
#include "window.h"

/**
 * @brief Simple demo to show off creating a window, accessing the input
 * manager, and polling for events.
 */

int main(void) {
  engine::EngineConfig engine_config;
  engine::Engine::Init(engine_config);
  engine::Window& window = engine::Engine::window();
  engine::InputManager& input_manager = engine::InputManager::Get();
  while (true) {
    window.PollEvents();

    if (input_manager.IsKeyPressed(engine::KeyCode::KC_W)) {
      std::cout << "W was pressed\n";
    }

    if (input_manager.IsKeyDown(engine::KeyCode::KC_S)) {
      std::cout << "S is down\n";
    }

    if (input_manager.IsKeyReleased(engine::KeyCode::KC_A)) {
      std::cout << "A was released\n";
    }
  }
  return 0;
}
