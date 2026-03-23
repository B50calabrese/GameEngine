#include "character_renderer.h"
#include <engine/graphics/renderer.h>

namespace tactical_rpg {

void CharacterRenderer::Render(const std::vector<Character>& party,
                              const std::vector<Character>& enemies,
                              Character* active_character,
                              const glm::vec2& offset,
                              float tile_size) {
    for(const auto& p : party) {
        if(p.is_downed) continue;
        glm::vec2 pos = offset + glm::vec2(p.grid_pos.x * tile_size, p.grid_pos.y * tile_size);
        engine::graphics::Renderer::Get().DrawQuad(pos + glm::vec2(5, 5), {tile_size-12, tile_size-12}, {0, 1, 0, 1});
    }
    for(const auto& e : enemies) {
        if(e.is_downed) continue;
        glm::vec2 pos = offset + glm::vec2(e.grid_pos.x * tile_size, e.grid_pos.y * tile_size);
        engine::graphics::Renderer::Get().DrawQuad(pos + glm::vec2(5, 5), {tile_size-12, tile_size-12}, {1, 0, 0, 1});
    }

    if (active_character) {
        glm::vec2 pos = offset + glm::vec2(active_character->grid_pos.x * tile_size, active_character->grid_pos.y * tile_size);
        engine::graphics::Renderer::Get().DrawQuad(pos, {tile_size-2, tile_size-2}, {1, 1, 0, 0.3f});
    }
}

} // namespace tactical_rpg
