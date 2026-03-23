#include "grid_renderer.h"
#include <engine/graphics/renderer.h>

namespace tactical_rpg {

void GridRenderer::Render(const BattleGrid& grid, const glm::vec2& offset,
                         float tile_size, const glm::ivec2& cursor_pos) {
  for (int y = 0; y < BattleGrid::kSize; ++y) {
    for (int x = 0; x < BattleGrid::kSize; ++x) {
      glm::vec2 pos = offset + glm::vec2(x * tile_size, y * tile_size);
      glm::vec4 color;
      switch (grid.GetTerrain(x, y)) {
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

      engine::graphics::Renderer::Get().DrawQuad(
          pos, {tile_size - 2, tile_size - 2}, color);
    }
  }
}

}  // namespace tactical_rpg
