/**
 * @file vertex2d.h
 * @brief Vertex layout for 2D rendering.
 */

#ifndef INCLUDE_ENGINE_GRAPHICS_VERTEX2D_H_
#define INCLUDE_ENGINE_GRAPHICS_VERTEX2D_H_

namespace engine::graphics {

/** @brief Vertex layout for PrimitiveRenderer. */
struct Vertex2D {
  float position[2];
  float color[4];
  float tex_coords[2];
  float tex_index;
  float is_font;
};

}  // namespace engine::graphics

#endif  // INCLUDE_ENGINE_GRAPHICS_VERTEX2D_H_
