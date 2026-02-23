#ifndef INCLUDE_GRAPHICS_TEXT_RENDERER_H_
#define INCLUDE_GRAPHICS_TEXT_RENDERER_H_

#include <ft2build.h>

#include <glm/glm.hpp>
#include <map>
#include <string>
#include FT_FREETYPE_H

namespace engine::graphics {

/**
 * @brief Represents a single character in a font file.
 */
struct Character {
  unsigned int texture_id;
  glm::ivec2 size;
  glm::ivec2 bearing;
  unsigned int advance;
};

/**
 * @brief Renders text using font files.
 */
class TextRenderer {
 public:
  /**
   * @brief Returns a reference to the singleton `TextRenderer` instance.
   * @return Reference to the TextRenderer.
   */
  static TextRenderer& Get() {
    static TextRenderer instance;
    return instance;
  }

  // Deleted copy/assignment for singleton integrity
  TextRenderer(const TextRenderer&) = delete;
  void operator=(const TextRenderer&) = delete;

  /**
   * @brief Initializes the TextRenderer.
   */
  void Init();

  /**
   * @brief Cleans up the TextRenderer.
   */
  void Shutdown();

  /**
   * @brief Loads a given font from a file.
   *
   * @param name The name
   * of the font when referenced.
   * @param path The location of the file path
   * with the font file.
   * @param font_size The size of the font.
   */
  void LoadFont(const std::string& name, const std::string& path,
                unsigned int font_size);

  /**
   * @brief Renders text with full transformation support.
   */
  void DrawText(const std::string& font_name, const std::string& text,
                glm::vec2 position, float rotation, float scale,
                const glm::vec4& color);

 private:
  TextRenderer() : ft_library_(nullptr) {}

  FT_Library ft_library_;
  std::map<std::string, std::map<char, Character>> fonts_;
};

}  // namespace engine::graphics

#endif  // INCLUDE_GRAPHICS_TEXT_RENDERER_H_
