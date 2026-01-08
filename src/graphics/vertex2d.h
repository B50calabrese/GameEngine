#ifndef VERTEX2D_H
#define VERTEX2D_H

namespace engine::graphics {

/**
 * Internal vertex structure for 2D batching.
 */
struct Vertex2D {
  float position[2];
  float color[4];
  float texCoords[2];
};

}  // namespace engine::graphics

#endif  // VERTEX2D_H
