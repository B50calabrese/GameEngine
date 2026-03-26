/**
 * @file vertex2d.h
 * @brief Vertex layout for 2D rendering.
 */

#ifndef SRC_ENGINE_GRAPHICS_VERTEX2D_H_
#define SRC_ENGINE_GRAPHICS_VERTEX2D_H_

namespace engine::graphics {

/**
 * @brief This layout must perfectly match the layout defined in the
 * PrimitiveRenderer's shader.
 *
 * Location 0: Position (vec2)
 * Location 1: Color (vec4)
 * Location 2: TexCoord (vec2)
 * Location 3: TexIndex (float)
 */
struct Vertex2D {
  /** @brief [x, y] coordinates. */
  float position[2];
  /** @brief [r, g, b, a] color. */
  float color[4];
  /** @brief [u, v] texture coordinates. */
  float tex_coords[2];
  /** @brief Index into the texture slots array. */
  float tex_index;
  /** @brief 1.0 if it's a font, 0.0 otherwise. */
  float is_font;
};

}  // namespace engine::graphics

#endif  // SRC_ENGINE_GRAPHICS_VERTEX2D_H_
