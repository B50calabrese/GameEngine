#include "buffer_utils.h"

#include <glad/glad.h>

namespace engine::graphics {

void BufferUtils::create_basic_buffers(unsigned int& vao, unsigned int& vbo,
                                       unsigned int& ebo, size_t vbo_size) {
  // Generate the handles
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &ebo);

  // Bind VAO first so it records the subsequent buffer bindings
  glBindVertexArray(vao);

  // Setup VBO (Vertex Buffer)
  glBindBuffer(GL_ARRAY_BUFFER, vbo);

  // We initialize with NULL and DYNAMIC_DRAW because renderers usually
  // update vertex data every frame (batching)
  glBufferData(GL_ARRAY_BUFFER, vbo_size, nullptr, GL_DYNAMIC_DRAW);

  // Setup EBO (Element/Index Buffer)
  // Note: The actual index data is usually static and provided
  // by the specific renderer implementation after this call.
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
}

void BufferUtils::set_attribute(unsigned int index, int size, size_t stride,
                                size_t offset) {
  // Enable the specific attribute slot in the currently bound VAO
  glEnableVertexAttribArray(index);

  // Define how OpenGL should interpret the vertex data
  // We assume GL_FLOAT as it is the standard for engine vertex data
  glVertexAttribPointer(
      index,            // Attribute index (location in shader)
      size,             // Number of components (e.g., 2 for vec2, 4 for vec4)
      GL_FLOAT,         // Data type
      GL_FALSE,         // Normalized?
      (GLsizei)stride,  // Byte offset between consecutive vertices
      (void*)offset     // Offset of the first component
  );
}

}  // namespace engine::graphics
