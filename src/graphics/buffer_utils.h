#ifndef SRC_GRAPHICS_BUFFER_UTILS_H_
#define SRC_GRAPHICS_BUFFER_UTILS_H_

#include <cstddef>

namespace engine::graphics {

/**
 * @brief A collection of functions to reduce OpenGL boilerplate related to
 * buffers.
 */
struct BufferUtils {
  /**
   * @brief Creates and binds a VAO, VBO, and EBO in one go leaving VAO bound.
   *
   * @param vao Reference to store the VAO ID.
   * @param vbo Reference to store the VBO ID.
   * @param ebo Reference to store the EBO ID.
   * @param vbo_size Size of the VBO in bytes.
   */
  static void CreateBasicBuffers(unsigned int& vao, unsigned int& vbo,
                                 unsigned int& ebo, size_t vbo_size);

  /**
   * @brief Defines a float vertex attribute pointer.
   *
   * @param index Attribute index.
   * @param size Number of components.
   * @param stride Stride between vertices.
   * @param offset Offset of the attribute.
   */
  static void SetAttribute(unsigned int index, int size, size_t stride,
                           size_t offset);
};
}  // namespace engine::graphics

#endif  // SRC_GRAPHICS_BUFFER_UTILS_H_
