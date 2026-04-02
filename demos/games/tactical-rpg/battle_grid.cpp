#include "battle_grid.h"

#include <algorithm>
#include <random>

#include "components.h"

namespace tactical_rpg {

void BattleGrid::Setup(engine::ecs::Registry& registry,
                       const GridConfig& config) {
  std::mt19937 gen(std::random_device{}());
  std::uniform_real_distribution<float> dist(0.0f, 1.0f);

  auto grid_entity = registry.CreateEntity();
  GridMapComponent grid_map;

  for (int y = 0; y < kSize; ++y) {
    for (int x = 0; x < kSize; ++x) {
      auto tile_entity = registry.CreateEntity();
      TerrainType terrain = TerrainType::Normal;

      float r = dist(gen);
      if (r < config.slow_chance)
        terrain = TerrainType::Slow;
      else if (r < config.slow_chance + config.damage_chance)
        terrain = TerrainType::Damage;
      else if (r < config.slow_chance + config.damage_chance +
                       config.impassible_chance)
        terrain = TerrainType::Impassible;

      // Safety areas
      if (y <= 1 || y >= kSize - 2) {
        terrain = TerrainType::Normal;
      }

      registry.AddComponent(tile_entity, TileComponent{terrain, {x, y}});
      grid_map.tiles[y][x] = tile_entity;
    }
  }

  registry.AddComponent(grid_entity, grid_map);
}

TerrainType BattleGrid::GetTerrain(engine::ecs::Registry& registry, int x,
                                   int y) {
  if (!IsInBounds(x, y)) return TerrainType::Impassible;

  auto view = registry.GetView<GridMapComponent>();
  for (auto entity : view) {
    auto& grid_map = registry.GetComponent<GridMapComponent>(entity);
    auto tile_entity = grid_map.tiles[y][x];
    if (registry.HasComponent<TileComponent>(tile_entity)) {
      return registry.GetComponent<TileComponent>(tile_entity).terrain;
    }
  }
  return TerrainType::Impassible;
}

bool BattleGrid::IsWalkable(engine::ecs::Registry& registry, int x, int y) {
  return IsInBounds(x, y) &&
         GetTerrain(registry, x, y) != TerrainType::Impassible;
}

bool BattleGrid::IsInBounds(int x, int y) {
  return x >= 0 && x < kSize && y >= 0 && y < kSize;
}

}  // namespace tactical_rpg
