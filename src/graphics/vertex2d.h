#ifndef VERTEX2D_H
#define VERTEX2D_H

namespace engine::graphics {

/**
 * This layout must perfectly match the layout defined in the
 * PrimitiveRenderer's shader:
 * Location 0: Position (vec2)
 * Location 1: Color (vec4)
 * Location 2: TexCoord (vec2)
 * Location 3: TexIndex (float)
 */
struct Vertex2D {
  float position[2];
  float color[4];
  float texCoords[2];
  float texIndex;
};

}  // namespace engine::graphics

#endif  // VERTEX2D_H