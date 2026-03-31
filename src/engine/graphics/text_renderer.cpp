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
#include <engine/graphics/utils/render_queue.h>
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
                            float rotation, float scale, const glm::vec4& color,
                            float z_index) {
  if (fonts_.find(font_name) == fonts_.end()) {
    return;
  }
  auto& characters = fonts_[font_name]->characters();
  float x_cursor = 0.0f;
  for (char c : text) {
    auto it = characters.find(c);
    if (it == characters.end()) {
      continue;
    }
    const Character& ch = it->second;
    float xpos = position.x + (x_cursor + ch.bearing.x) * scale;
    float ypos = position.y - (ch.size.y - ch.bearing.y) * scale;
    float w = ch.size.x * scale;
    float h = ch.size.y * scale;
    glm::vec2 char_rel_pos = glm::vec2(xpos - position.x, ypos - position.y);
    if (rotation != 0.0f) {
      float rad = glm::radians(rotation);
      float cos_a = std::cos(rad);
      float sin_a = std::sin(rad);
      float rx = char_rel_pos.x * cos_a - char_rel_pos.y * sin_a;
      float ry = char_rel_pos.x * sin_a + char_rel_pos.y * cos_a;
      char_rel_pos = glm::vec2(rx, ry);
    }

    utils::RenderCommand cmd;
    cmd.z_order = z_index;
    cmd.texture_id = ch.texture_id;
    cmd.position = position + char_rel_pos;
    cmd.size = {w, h};
    cmd.color = color;
    cmd.rotation = rotation;
    cmd.uv_min = {0.0f, 1.0f};
    cmd.uv_max = {1.0f, 0.0f};
    cmd.is_font = true;
    graphics::utils::RenderQueue::Default().Submit(cmd);

    x_cursor += (ch.advance >> 6);
  }
}

}  // namespace engine::graphics
