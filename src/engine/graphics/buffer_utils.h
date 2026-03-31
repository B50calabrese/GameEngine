/**
 * @file buffer_utils.h
 * @brief Internal OpenGL buffer management helpers.
 */

#ifndef SRC_ENGINE_GRAPHICS_BUFFER_UTILS_H_
#define SRC_ENGINE_GRAPHICS_BUFFER_UTILS_H_

#include <cstddef>

namespace engine::graphics {

/**
 * @brief OpenGL buffer management boilerplate reduction.
 */
struct BufferUtils {
  static void CreateBasicBuffers(unsigned int& vao, unsigned int& vbo,
                                 unsigned int& ebo, size_t vbo_size);
  static void SetAttribute(unsigned int index, int size, size_t stride,
                           size_t offset);
};

}  // namespace engine::graphics

#endif  // SRC_ENGINE_GRAPHICS_BUFFER_UTILS_H_
