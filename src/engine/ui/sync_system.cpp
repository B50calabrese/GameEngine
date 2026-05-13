/**
 * @file sync_system.cpp
 * @brief Implementation of the UI sync system.
 */

#include <engine/ui/sync_system.h>

#include <engine/ecs/components/text.h>
#include <engine/ecs/components/ui_binding.h>

namespace engine::ui {

using namespace engine::ecs::components;

void UiSyncSystem::Update(ecs::Registry& reg) {
  auto view = reg.GetView<UiBinding, Text>();
  for (auto entity : view) {
    if (!reg.IsAlive(entity) || !reg.HasComponent<UiBinding>(entity) ||
        !reg.HasComponent<Text>(entity))
      continue;
    auto& binding = reg.GetComponent<UiBinding>(entity);
    auto& text = reg.GetComponent<Text>(entity);
    if (binding.get_text) {
      std::string current = binding.get_text();
      if (current != binding.last_value) {
        text.content = current;
        binding.last_value = current;
      }
    }
  }
}

}  // namespace engine::ui
