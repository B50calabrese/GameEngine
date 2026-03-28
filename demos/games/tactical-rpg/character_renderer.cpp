#include "character_renderer.h"

#include <engine/graphics/renderer.h>

#include "components.h"

namespace tactical_rpg {

void CharacterRenderer::Render(engine::ecs::Registry& registry,
                               engine::ecs::EntityID active_char,
                               const glm::vec2& offset, float tile_size) {
  auto view = registry.GetView<GridPositionComponent, IdentityComponent,
                               TurnStateComponent>();
  for (auto entity : view) {
    auto& grid_pos = registry.GetComponent<GridPositionComponent>(entity);
    auto& identity = registry.GetComponent<IdentityComponent>(entity);
    auto& turn_state = registry.GetComponent<TurnStateComponent>(entity);

    if (turn_state.is_downed) continue;

    glm::vec2 pos = offset + glm::vec2(grid_pos.pos.x * tile_size,
                                       grid_pos.pos.y * tile_size);
    glm::vec4 color =
        identity.is_enemy ? glm::vec4(1, 0, 0, 1) : glm::vec4(0, 1, 0, 1);

    engine::graphics::Renderer::Get().DrawQuad(
        pos + glm::vec2(5, 5), {tile_size - 12, tile_size - 12}, color);

    if (entity == active_char) {
      engine::graphics::Renderer::Get().DrawQuad(
          pos, {tile_size - 2, tile_size - 2}, {1, 1, 0, 0.3f});
    }
  }
}

}  // namespace tactical_rpg
