/**
 * @file sprite_sheet.h
 * @brief Sprite sheet resource management.
 */

#ifndef INCLUDE_ENGINE_GRAPHICS_SPRITE_SHEET_H_
#define INCLUDE_ENGINE_GRAPHICS_SPRITE_SHEET_H_

#include <memory>
#include <string>
#include <vector>

#include <glm/glm.hpp>

namespace engine::graphics {

class Texture;

/**
 * @brief Manages a texture divided into a grid of sprites.
 *
 * Sprites are indexed from left-to-right, top-to-bottom starting at 0.
 * Index 0 refers to the top-left sprite in the texture.
 */
class SpriteSheet {
 public:
  /**
   * @brief Loads a sprite sheet.
   *
   * Format: "path:sprite_width:sprite_height:rows:cols"
   *
   * @param path The path and parameters.
   * @return A shared pointer to the SpriteSheet, or nullptr if loading fails.
   */
  static std::shared_ptr<SpriteSheet> Load(const std::string& path);

  /**
   * @brief Creates a SpriteSheet from an existing texture.
   * @param texture The texture to use.
   * @param sprite_width Width of each sprite.
   * @param sprite_height Height of each sprite.
   * @param rows Number of rows.
   * @param cols Number of columns.
   * @return A shared pointer to the SpriteSheet.
   */
  static std::shared_ptr<SpriteSheet> Create(std::shared_ptr<Texture> texture,
                                             int sprite_width,
                                             int sprite_height, int rows,
                                             int cols);

  /**
   * @brief Calculates the UV coordinates for a given sprite index.
   * @param index The 0-based index of the sprite (row-major, top-left origin).
   * @param uv_min Output for bottom-left UV coordinate.
   * @param uv_max Output for top-right UV coordinate.
   */
  void GetUVs(int index, glm::vec2* uv_min, glm::vec2* uv_max) const;

  /** @brief Gets the underlying texture. */
  [[nodiscard]] const Texture* texture() const { return texture_.get(); }

  /** @brief Gets the sprite width. */
  [[nodiscard]] int sprite_width() const { return sprite_width_; }

  /** @brief Gets the sprite height. */
  [[nodiscard]] int sprite_height() const { return sprite_height_; }

  /** @brief Gets the number of rows. */
  [[nodiscard]] int rows() const { return rows_; }

  /** @brief Gets the number of columns. */
  [[nodiscard]] int cols() const { return cols_; }

 private:
  SpriteSheet(std::shared_ptr<Texture> texture, int sprite_width,
              int sprite_height, int rows, int cols)
      : texture_(std::move(texture)),
        sprite_width_(sprite_width),
        sprite_height_(sprite_height),
        rows_(rows),
        cols_(cols) {}

  std::shared_ptr<Texture> texture_;
  int sprite_width_;
  int sprite_height_;
  int rows_;
  int cols_;
};

}  // namespace engine::graphics

#endif  // INCLUDE_ENGINE_GRAPHICS_SPRITE_SHEET_H_
