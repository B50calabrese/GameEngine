#include "battle_grid.h"

#include <algorithm>
#include <random>

#include <engine/graphics/renderer.h>

namespace tactical_rpg {

BattleGrid::BattleGrid() { Setup(); }

void BattleGrid::Setup() {
  std::mt19937 gen(std::random_device{}());
  std::uniform_real_distribution<float> dist(0.0f, 1.0f);

  for (int y = 0; y < kSize; ++y) {
    for (int x = 0; x < kSize; ++x) {
      float r = dist(gen);
      if (r < 0.1f)
        grid_[y][x] = TerrainType::Slow;
      else if (r < 0.15f)
        grid_[y][x] = TerrainType::Damage;
      else if (r < 0.2f)
        grid_[y][x] = TerrainType::Impassible;
      else
        grid_[y][x] = TerrainType::Normal;
    }
  }

  // Safety areas
  for (int x = 0; x < kSize; ++x) {
    grid_[0][x] = TerrainType::Normal;
    grid_[1][x] = TerrainType::Normal;
    grid_[kSize - 1][x] = TerrainType::Normal;
    grid_[kSize - 2][x] = TerrainType::Normal;
  }
}

TerrainType BattleGrid::GetTerrain(int x, int y) const {
  if (IsInBounds(x, y)) return grid_[y][x];
  return TerrainType::Impassible;
}

bool BattleGrid::IsWalkable(int x, int y) const {
  return IsInBounds(x, y) && grid_[y][x] != TerrainType::Impassible;
}

bool BattleGrid::IsInBounds(int x, int y) const {
  return x >= 0 && x < kSize && y >= 0 && y < kSize;
}

void BattleGrid::Render(const glm::vec2& offset, float tile_size,
                        const glm::ivec2& cursor_pos) {
  for (int y = 0; y < kSize; ++y) {
    for (int x = 0; x < kSize; ++x) {
      glm::vec2 pos = offset + glm::vec2(x * tile_size, y * tile_size);
      glm::vec4 color;
      switch (grid_[y][x]) {
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
