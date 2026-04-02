#include "grid_renderer.h"

#include <engine/graphics/renderer.h>

#include "components.h"

namespace tactical_rpg {

void GridRenderer::Render(engine::ecs::Registry& registry,
                          const glm::vec2& offset, float tile_size,
                          const glm::ivec2& cursor_pos) {
  // Optimization: Pre-resolve texture paths if not cached by engine
  static auto floor_tex = engine::graphics::Texture::Load("textures/floor.png");
  static auto slow_tex = engine::graphics::Texture::Load("textures/wall.png");
  static auto dmg_tex = engine::graphics::Texture::Load("textures/acid.png");
  static auto wall_tex = engine::graphics::Texture::Load("textures/wall.png");

  auto view = registry.GetView<GridMapComponent>();
  for (auto grid_entity : view) {
    auto& grid_map = registry.GetComponent<GridMapComponent>(grid_entity);
    for (int y = 0; y < GridMapComponent::kSize; ++y) {
      for (int x = 0; x < GridMapComponent::kSize; ++x) {
        auto tile_entity = grid_map.tiles[y][x];
        if (!registry.HasComponent<TileComponent>(tile_entity)) continue;
        auto& tile = registry.GetComponent<TileComponent>(tile_entity);

        glm::vec2 pos = offset + glm::vec2(x * tile_size, y * tile_size);
        engine::graphics::Texture* tex = floor_tex.get();
        switch (tile.terrain) {
          case TerrainType::Normal: tex = floor_tex.get(); break;
          case TerrainType::Slow: tex = slow_tex.get(); break;
          case TerrainType::Damage: tex = dmg_tex.get(); break;
          case TerrainType::Impassible: tex = wall_tex.get(); break;
        }

        if (tex) {
          engine::graphics::Renderer::Get().DrawTexturedQuad(
              pos + glm::vec2(tile_size/2), {tile_size, tile_size}, tex);
        } else {
           engine::graphics::Renderer::Get().DrawQuad(
            pos, {tile_size - 2, tile_size - 2}, {0.2f, 0.2f, 0.2f, 1.0f});
        }

        if (x == cursor_pos.x && y == cursor_pos.y) {
           engine::graphics::Renderer::Get().DrawQuad(
            pos, {tile_size - 2, tile_size - 2}, {1.0f, 1.0f, 1.0f, 0.3f});
        }
      }
    }
  }
}

}  // namespace tactical_rpg
