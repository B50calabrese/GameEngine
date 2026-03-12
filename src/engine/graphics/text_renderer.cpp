/**
 * @file text_renderer.cpp
 * @brief TextRenderer class implementation.
 */

#include <iostream>

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

#include <engine/graphics/primitive_renderer.h>
#include <engine/graphics/renderer.h>
#include <engine/graphics/text_renderer.h>
#include <engine/util/asset_manager.h>

namespace engine::graphics {

void TextRenderer::Init() {}

void TextRenderer::Shutdown() { fonts_.clear(); }

void TextRenderer::LoadFont(const std::string& name, const std::string& path,
                            unsigned int font_size) {
  std::string full_path = path + ":" + std::to_string(font_size);
  std::shared_ptr<Font> font = util::AssetManager<Font>::Get(full_path);
  if (font) {
    fonts_[name] = font;
  }
}

void TextRenderer::AddFont(const std::string& name,
                           std::shared_ptr<Font> font) {
  fonts_[name] = font;
}

void TextRenderer::DrawText(const std::string& font_name,
                            const std::string& text, const glm::vec2& position,
                            float rotation, float scale,
                            const glm::vec4& color) {
  if (fonts_.find(font_name) == fonts_.end()) {
    return;
  }

  auto& characters = fonts_[font_name]->characters();
  float x_cursor = 0.0f;

  for (char c : text) {
    if (characters.find(c) == characters.end()) {
      continue;
    }
    Character ch = characters.at(c);

    // Character bearing: horizontal offset from origin to left of glyph,
    // and vertical offset from origin to top of glyph.
    float xpos = position.x + (x_cursor + ch.bearing.x) * scale;
    float ypos = position.y - (ch.size.y - ch.bearing.y) * scale;

    float w = ch.size.x * scale;
    float h = ch.size.y * scale;

    // submit_textured_quad handles rotation around the specified origin.
    // Calculate character position relative to text origin
    glm::vec2 char_rel_pos = glm::vec2(xpos - position.x, ypos - position.y);

    if (rotation != 0.0f) {
      float rad = glm::radians(rotation);
      float cos_a = std::cos(rad);
      float sin_a = std::sin(rad);
      float rx = char_rel_pos.x * cos_a - char_rel_pos.y * sin_a;
      float ry = char_rel_pos.x * sin_a + char_rel_pos.y * cos_a;
      char_rel_pos = glm::vec2(rx, ry);
    }

    PrimitiveRenderer::SubmitTexturedQuad(position + char_rel_pos, {w, h},
                                          ch.texture_id, color, rotation,
                                          {0.0f, 0.0f}, true);

    x_cursor += (ch.advance >> 6);
  }
}

}  // namespace engine::graphics
