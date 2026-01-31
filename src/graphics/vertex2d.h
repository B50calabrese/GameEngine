#ifndef SRC_GRAPHICS_VERTEX2D_H_
#define SRC_GRAPHICS_VERTEX2D_H_

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
  float texCoords[2];
  /** @brief Index into the texture slots array. */
  float texIndex;
};

}  // namespace engine::graphics

#endif  // SRC_GRAPHICS_VERTEX2D_H_
