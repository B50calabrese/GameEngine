#ifndef BUFFER_UTILS_H
#define BUFFER_UTILS_H

namespace engine::graphics {

/**
 * A collection of functions to reduce OpenGL boilerplate related to buffers.
 */
struct BufferUtils {
  /**
   * Creates and binds a VAO, VBO, and EBO in one go leaving VA) bound.
   */
  static void CreateBasicBuffers(unsigned int& vao, unsigned int& vbo,
                                 unsigned int& ebo, size_t vboSize);

  /**
   * Defines a float vertex attribute pointer.
   */
  static void SetAttribute(unsigned int index, int size, size_t stride,
                           size_t offset);
};
}  // namespace engine::graphics

#endif  // BUFFER_UTILS_H