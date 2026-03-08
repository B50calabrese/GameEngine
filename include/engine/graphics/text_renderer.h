/**
 * @file text_renderer.h
 * @brief Specialized renderer for text.
 */

#ifndef INCLUDE_ENGINE_GRAPHICS_TEXT_RENDERER_H_
#define INCLUDE_ENGINE_GRAPHICS_TEXT_RENDERER_H_

#include <glm/glm.hpp>
#include <map>
#include <memory>
#include <string>

#include <engine/graphics/font.h>

/**
 * @namespace engine::graphics
 * @brief Graphics and rendering systems.
 */
namespace engine::graphics {

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
   * @brief Caches a font by name.
   *
   * @param name The name to associate with the font.
   * @param path The file path of the font.
   * @param font_size The pixel size to load.
   */
  void LoadFont(const std::string& name, const std::string& path,
                unsigned int font_size);

  /**
   * @brief Caches an already loaded font resource.
   *
   * @param name The name to associate with the font.
   * @param font The Font resource.
   */
  void AddFont(const std::string& name, std::shared_ptr<Font> font);

  /**
   * @brief Renders text with full transformation support.
   */
  void DrawText(const std::string& font_name, const std::string& text,
                const glm::vec2& position, float rotation, float scale,
                const glm::vec4& color);

 private:
  TextRenderer() = default;

  std::map<std::string, std::shared_ptr<Font>> fonts_;
};

}  // namespace engine::graphics

#endif  // INCLUDE_ENGINE_GRAPHICS_TEXT_RENDERER_H_
