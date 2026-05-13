/**
 * @file input_system.cpp
 * @brief Implementation of the UI input system.
 */

#include <engine/ui/input_system.h>

#include <engine/ecs/components/ui_interactable.h>
#include <engine/ecs/components/ui_transform.h>
#include <engine/input/input_manager.h>

namespace engine::ui {

using namespace engine::ecs::components;

void UiInputSystem::Update(ecs::Registry& reg) {
  auto& input = InputManager::Get();
  glm::vec2 mouse_pos = input.mouse_screen_pos();
  bool mouse_pressed = input.IsKeyDown(KeyCode::kMouseLeft);

  auto view = reg.GetView<UiTransform, UiInteractable>();
  for (auto entity : view) {
    if (!reg.IsAlive(entity) || !reg.HasComponent<UiTransform>(entity) ||
        !reg.HasComponent<UiInteractable>(entity))
      continue;
    auto& transform = reg.GetComponent<UiTransform>(entity);
    auto& interactable = reg.GetComponent<UiInteractable>(entity);

    bool was_hovered = interactable.is_hovered;

    interactable.is_hovered =
        mouse_pos.x >= transform.global_pos.x &&
        mouse_pos.x <= transform.global_pos.x + transform.size.x &&
        mouse_pos.y >= transform.global_pos.y &&
        mouse_pos.y <= transform.global_pos.y + transform.size.y;

    if (interactable.is_hovered != was_hovered) {
      if (interactable.on_hover_changed) {
        interactable.on_hover_changed(interactable.is_hovered);
      }
    }

    if (interactable.is_hovered) {
      input.Consume();

      if (mouse_pressed && !interactable.is_pressed) {
        interactable.is_pressed = true;
      } else if (!mouse_pressed && interactable.is_pressed) {
        interactable.is_pressed = false;
        if (interactable.on_click) {
          interactable.on_click();
        }
      }
    } else {
      interactable.is_pressed = false;
    }
  }
}

}  // namespace engine::ui
