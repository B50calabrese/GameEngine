#ifndef DEMOS_GAMES_TACTICAL_RPG_CHARACTER_RENDERER_H_
#define DEMOS_GAMES_TACTICAL_RPG_CHARACTER_RENDERER_H_

#include <vector>

#include <engine/ecs/registry.h>
#include <glm/glm.hpp>

#include "game_types.h"

namespace tactical_rpg {

class CharacterRenderer {
 public:
  static void Render(engine::ecs::Registry& registry,
                     engine::ecs::EntityID active_char, const glm::vec2& offset,
                     float tile_size);
};

}  // namespace tactical_rpg

#endif  // DEMOS_GAMES_TACTICAL_RPG_CHARACTER_RENDERER_H_
