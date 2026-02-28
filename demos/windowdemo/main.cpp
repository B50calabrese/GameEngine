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
  engine::Engine::init(engine_config);
  engine::Window& window = engine::Engine::window();
  engine::InputManager& input_manager = engine::InputManager::get();
  while (true) {
    window.poll_events();

    if (input_manager.is_key_pressed(engine::KeyCode::KC_W)) {
      std::cout << "W was pressed\n";
    }

    if (input_manager.is_key_down(engine::KeyCode::KC_S)) {
      std::cout << "S is down\n";
    }

    if (input_manager.is_key_released(engine::KeyCode::KC_A)) {
      std::cout << "A was released\n";
    }
  }
  return 0;
}
