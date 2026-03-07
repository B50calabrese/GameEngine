#include "graphics/font.h"

#include <ft2build.h>
#include <glad/glad.h>

#include <map>
#include <memory>
#include <string>

#include "graphics/renderer.h"
#include "util/logger.h"
#include FT_FREETYPE_H

namespace engine::graphics {

std::shared_ptr<Font> Font::Load(const std::string& path) {
  // Parse size if provided: "path.ttf:size"
  std::string file_path = path;
  unsigned int font_size = 16;
  size_t colon_pos = path.find_last_of(':');
  if (colon_pos != std::string::npos) {
    file_path = path.substr(0, colon_pos);
    try {
      font_size = std::stoi(path.substr(colon_pos + 1));
    } catch (...) {
      LOG_WARN("[Font] Failed to parse font size from: %s. Using default 16.",
               path.c_str());
    }
  }

  std::string full_path = Renderer::Get().ResolveAssetPath(file_path);

  FT_Library ft;
  if (FT_Init_FreeType(&ft)) {
    LOG_ERR("ERROR::FREETYPE: Could not init FreeType Library");
    return nullptr;
  }

  FT_Face face;
  if (FT_New_Face(ft, full_path.c_str(), 0, &face)) {
    LOG_ERR("ERROR::FREETYPE: Failed to load font: %s", full_path.c_str());
    FT_Done_FreeType(ft);
    return nullptr;
  }

  FT_Set_Pixel_Sizes(face, 0, font_size);

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  std::map<char, Character> char_map;
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

    char_map[c] = {
        texture,
        glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
        glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
        static_cast<unsigned int>(face->glyph->advance.x)};
  }

  FT_Done_Face(face);
  FT_Done_FreeType(ft);

  LOG_INFO("[Font] Loaded font from '%s' (%dpx)", full_path.c_str(), font_size);

  return std::shared_ptr<Font>(new Font(std::move(char_map)));
}

Font::~Font() {
  for (auto const& [c, character] : characters_) {
    glDeleteTextures(1, &character.texture_id);
  }
}

}  // namespace engine::graphics
