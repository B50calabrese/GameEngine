/**
 * @file vertex2d.h
 * @brief Vertex layout for 2D rendering.
 */

#ifndef SRC_ENGINE_GRAPHICS_VERTEX2D_H_
#define SRC_ENGINE_GRAPHICS_VERTEX2D_H_

#include <glm/glm.hpp>

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
  glm::vec2 position;
  /** @brief [r, g, b, a] color. */
  glm::vec4 color;
  /** @brief [u, v] texture coordinates. */
  glm::vec2 tex_coords;
  /** @brief Index into the texture slots array. */
  float tex_index;
};

}  // namespace engine::graphics

#endif  // SRC_ENGINE_GRAPHICS_VERTEX2D_H_
