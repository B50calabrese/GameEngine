#include "graphics/text_renderer.h"

#include <glad/glad.h>

#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "graphics/renderer.h"
#include "primitive_renderer.h"

namespace engine::graphics {

void TextRenderer::Init() {
  if (FT_Init_FreeType(&ft_library_)) {
    std::cout << "ERROR::FREETYPE: Could not init FreeType Library"
              << std::endl;
  }
}

void TextRenderer::Shutdown() {
  // Clean up OpenGL textures
  for (auto const& [fontName, charMap] : fonts_) {
    for (auto const& [c, character] : charMap) {
      glDeleteTextures(1, &character.texture_id);
    }
  }
  FT_Done_FreeType(ft_library_);
}

void TextRenderer::LoadFont(const std::string& name, const std::string& path,
                            unsigned int font_size) {
  std::string full_path = Renderer::Get().ResolveAssetPath(path);

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

void TextRenderer::DrawText(const std::string& font_name,
                            const std::string& text, glm::vec2 position,
                            float rotation, float scale,
                            const glm::vec4& color) {
  if (fonts_.find(font_name) == fonts_.end()) return;

  auto& characters = fonts_[font_name];
  float x_cursor = 0.0f;
  float rad = glm::radians(rotation);
  float cosA = cos(rad);
  float sinA = sin(rad);

  glm::mat4 rotationMat =
      glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0, 0, 1));

  for (char c : text) {
    Character ch = characters[c];

    // Calculate the position of the character's baseline origin relative to
    // start (x,y)
    float worldOriginX = position.x + (x_cursor * cosA);
    float worldOriginY = position.y + (x_cursor * sinA);

    float w = ch.size.x * scale;
    float h = ch.size.y * scale;

    // We pass the baseline origin as the (x,y) and the bearing as local offsets
    // SubmitTexturedQuad now handles the rotation of these offsets correctly.
    PrimitiveRenderer::SubmitTexturedQuad(worldOriginX, worldOriginY, w, h,
                                          ch.texture_id, &color[0], rotation,
                                          true);

    x_cursor += (ch.advance >> 6) * scale;
  }
}

}  // namespace engine::graphics
