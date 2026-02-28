#include "graphics/text_renderer.h"

#include <glad/glad.h>

#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "graphics/renderer.h"
#include "primitive_renderer.h"

namespace engine::graphics {

void TextRenderer::init() {
  if (FT_Init_FreeType(&ft_library_)) {
    std::cout << "ERROR::FREETYPE: Could not init FreeType Library"
              << std::endl;
  }
}

void TextRenderer::shutdown() {
  // Clean up OpenGL textures
  for (auto const& [fontName, charMap] : fonts_) {
    for (auto const& [c, character] : charMap) {
      glDeleteTextures(1, &character.texture_id);
    }
  }
  FT_Done_FreeType(ft_library_);
}

void TextRenderer::load_font(const std::string& name, const std::string& path,
                             unsigned int font_size) {
  std::string full_path = Renderer::get().resolve_asset_path(path);

  FT_Face face;
  if (FT_New_Face(ft_library_, full_path.c_str(), 0, &face)) {
    std::cout << "ERROR::FREETYPE: Failed to load font: " << full_path
              << std::endl;
    return;
  }

  FT_Set_Pixel_Sizes(face, 0, font_size);

  // Disable byte-alignment restriction (FreeType bitmaps are 1-byte aligned)
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  std::map<char, Character> charMap;
  for (unsigned char c = 0; c < 128; c++) {
    if (FT_Load_Char(face, c, FT_LOAD_RENDER)) continue;

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width,
                 face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE,
                 face->glyph->bitmap.buffer);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    charMap[c] = {
        texture,
        glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
        glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
        static_cast<unsigned int>(face->glyph->advance.x)};
  }

  fonts_[name] = charMap;
  FT_Done_Face(face);
  std::cout << "[FontRenderer] Loaded font '" << name << "' (" << font_size
            << "px)" << std::endl;
}

void TextRenderer::draw_text(const std::string& font_name,
                             const std::string& text, glm::vec2 position,
                             float rotation, float scale,
                             const glm::vec4& color) {
  if (fonts_.find(font_name) == fonts_.end()) return;

  auto& characters = fonts_[font_name];
  float x_cursor = 0.0f;

  for (char c : text) {
    Character ch = characters[c];

    // Character bearing: horizontal offset from origin to left of glyph,
    // and vertical offset from origin to top of glyph.
    float xpos = position.x + (x_cursor + ch.bearing.x) * scale;
    float ypos = position.y - (ch.size.y - ch.bearing.y) * scale;

    float w = ch.size.x * scale;
    float h = ch.size.y * scale;

    // submit_textured_quad handles rotation around the specified origin.
    // For text characters, we use the bottom-left of the glyph as the origin for its individual quad,
    // but the text as a whole should ideally rotate around the start position.
    // Implementing full text rotation is complex; for now we rotate each character quad
    // around the 'position' passed to draw_text.

    // Calculate character position relative to text origin
    glm::vec2 char_rel_pos = glm::vec2(xpos - position.x, ypos - position.y);

    if (rotation != 0.0f) {
        float rad = glm::radians(rotation);
        float cosA = cos(rad);
        float sinA = sin(rad);
        float rx = char_rel_pos.x * cosA - char_rel_pos.y * sinA;
        float ry = char_rel_pos.x * sinA + char_rel_pos.y * cosA;
        char_rel_pos = glm::vec2(rx, ry);
    }

    PrimitiveRenderer::submit_textured_quad(position + char_rel_pos,
                                            {w, h}, ch.texture_id, color,
                                            rotation, {0.0f, 0.0f}, true);

    x_cursor += (ch.advance >> 6);
  }
}

}  // namespace engine::graphics
