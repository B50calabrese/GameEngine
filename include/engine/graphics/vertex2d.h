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
 */
struct Vertex2D {
  /** @brief [x, y] world coordinates. */
  float position[2];
  /** @brief [r, g, b, a] primary color. */
  float color[4];
  /** @brief [r, g, b, a] secondary color for gradients. */
  float color2[4];
  /** @brief [u, v] texture coordinates. */
  float tex_coords[2];
  /** @brief [lx, ly] local coordinates for SDF calculations. */
  float local_pos[2];
  /** @brief Index into the texture slots array. */
  float tex_index;
  /** @brief 0: Quad, 1: Circle, 2: Triangle, 3: Line, 4: Point, 5: Polygon. */
  float shape_type;
  /** @brief Border thickness or line width. */
  float thickness;
  /** @brief Corner radius for rounded rectangles. */
  float roundness;
  /** @brief 0: None, 1: Linear, 2: Radial, 3: Vertex (default). */
  float gradient_type;
  /** @brief 1.0 if it's a font, 0.0 otherwise. */
  float is_font;
  /** @brief 1.0 if it's a dashed line, 0.0 otherwise. */
  float is_dashed;
};

}  // namespace engine::graphics

#endif  // SRC_ENGINE_GRAPHICS_VERTEX2D_H_
