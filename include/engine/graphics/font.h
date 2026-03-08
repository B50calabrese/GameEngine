/**
 * @file font.h
 * @brief Font resource for text rendering.
 */

#ifndef INCLUDE_ENGINE_GRAPHICS_FONT_H_
#define INCLUDE_ENGINE_GRAPHICS_FONT_H_

#include <glm/glm.hpp>
#include <map>
#include <memory>
#include <string>

/**
 * @namespace engine::graphics
 * @brief Graphics and rendering systems.
 */
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
 * @brief Represents a loaded font resource.
 */
class Font {
 public:
  /**
   * @brief Loads a font from disk.
   *
   * Expected format for path: "filename.ttf" or "filename.ttf:size".
   * If size is not provided, a default (e.g., 16) is used.
   *
   * @param path The path (and optional size) of the font.
   * @return A shared pointer to the Font, or nullptr if loading fails.
   */
  static std::shared_ptr<Font> Load(const std::string& path);

  /** @brief Destructor cleans up OpenGL resources. */
  ~Font();

  /**
   * @brief Gets the character map for this font.
   * @return Reference to the character map.
   */
  const std::map<char, Character>& characters() const { return characters_; }

 private:
  explicit Font(std::map<char, Character> characters)
      : characters_(std::move(characters)) {}

  std::map<char, Character> characters_;
};

}  // namespace engine::graphics

#endif  // INCLUDE_ENGINE_GRAPHICS_FONT_H_
