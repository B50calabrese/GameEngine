#include "battle_grid.h"

#include <algorithm>
#include <random>

namespace tactical_rpg {

BattleGrid::BattleGrid() { Setup(); }

void BattleGrid::Setup(const GridConfig& config) {
  std::mt19937 gen(std::random_device{}());
  std::uniform_real_distribution<float> dist(0.0f, 1.0f);

  for (int y = 0; y < kSize; ++y) {
    for (int x = 0; x < kSize; ++x) {
      float r = dist(gen);
      if (r < config.slow_chance)
        grid_[y][x] = TerrainType::Slow;
      else if (r < config.slow_chance + config.damage_chance)
        grid_[y][x] = TerrainType::Damage;
      else if (r < config.slow_chance + config.damage_chance +
                       config.impassible_chance)
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

}  // namespace tactical_rpg
