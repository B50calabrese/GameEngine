#include "grid_renderer.h"

#include <engine/graphics/renderer.h>

#include "components.h"

namespace tactical_rpg {

void GridRenderer::Render(engine::ecs::Registry& registry,
                          const glm::vec2& offset, float tile_size,
                          const glm::ivec2& cursor_pos) {
  auto view = registry.GetView<GridMapComponent>();
  for (auto grid_entity : view) {
    auto& grid_map = registry.GetComponent<GridMapComponent>(grid_entity);
    for (int y = 0; y < GridMapComponent::kSize; ++y) {
      for (int x = 0; x < GridMapComponent::kSize; ++x) {
        auto tile_entity = grid_map.tiles[y][x];
        if (!registry.HasComponent<TileComponent>(tile_entity)) continue;
        auto& tile = registry.GetComponent<TileComponent>(tile_entity);

        glm::vec2 pos = offset + glm::vec2(x * tile_size, y * tile_size);
        glm::vec4 color;
        switch (tile.terrain) {
          case TerrainType::Normal:
            color = {0.2f, 0.2f, 0.2f, 1.0f};
            break;
          case TerrainType::Slow:
            color = {0.4f, 0.4f, 0.2f, 1.0f};
            break;
          case TerrainType::Damage:
            color = {0.5f, 0.1f, 0.1f, 1.0f};
            break;
          case TerrainType::Impassible:
            color = {0.1f, 0.1f, 0.1f, 1.0f};
            break;
        }

        if (x == cursor_pos.x && y == cursor_pos.y) {
          color += glm::vec4(0.3f, 0.3f, 0.3f, 0.0f);
        }

        engine::graphics::utils::RenderQueue::Default().Submit(
            pos, {tile_size - 2, tile_size - 2}, color);
      }
    }
  }
}

}  // namespace tactical_rpg
