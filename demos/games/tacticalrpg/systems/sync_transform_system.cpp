#include "sync_transform_system.h"

#include <engine/ecs/components/light.h>
#include <engine/ecs/components/occluder.h>
#include <engine/ecs/components/quad.h>
#include <engine/ecs/components/sprite.h>
#include <engine/ecs/components/text.h>
#include <engine/ecs/components/transform.h>

#include "../components.h"

namespace tactical_rpg {

void SyncTransformSystem::Update(engine::ecs::Registry& registry,
                                 float tile_size,
                                 const glm::vec2& grid_offset) {
  auto view = registry.GetView<GridPositionComponent,
                               ::engine::ecs::components::Transform>();
  for (auto entity : view) {
    auto& grid_pos = registry.GetComponent<GridPositionComponent>(entity);
    auto& transform =
        registry.GetComponent<::engine::ecs::components::Transform>(entity);

    transform.position.x =
        grid_offset.x + grid_pos.pos.x * tile_size + tile_size / 2.0f;
    transform.position.y =
        grid_offset.y + grid_pos.pos.y * tile_size + tile_size / 2.0f;
  }
}

}  // namespace tactical_rpg
