// Copyright 2024 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <iostream>

#include "game_engine/engine.h"
#include "game_engine/input_manager.h"
#include "game_engine/window.h"

/**
 * @brief A simple demo application to showcase the game engine.
 *
 * This demo initializes the engine, creates a window, and polls for input
 * events.
 */
int main(void) {
  game_engine::EngineConfig engine_config;
  game_engine::Engine::Init(engine_config);
  game_engine::Window& window = game_engine::Engine::GetWindow();
  game_engine::InputManager& input_manager = game_engine::InputManager::Get();

  while (true) {
    window.PollEvents();

    if (input_manager.IsKeyPressed(game_engine::KeyCode::kKeyW)) {
      std::cout << "W was pressed\n";
    }

    if (input_manager.IsKeyDown(game_engine::KeyCode::kKeyS)) {
      std::cout << "S is down\n";
    }

    if (input_manager.IsKeyReleased(game_engine::KeyCode::kKeyA)) {
      std::cout << "A was released\n";
    }
  }

  return 0;
}
