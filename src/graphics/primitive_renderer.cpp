#include "primitive_renderer.h"

#include <glad/glad.h>

#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>

#include "buffer_utils.h"
#include "graphics/shader.h"

namespace engine::graphics {

unsigned int PrimitiveRenderer::vao = 0;
unsigned int PrimitiveRenderer::vbo = 0;
unsigned int PrimitiveRenderer::ebo = 0;
std::unique_ptr<Shader> PrimitiveRenderer::default_shader = nullptr;
std::vector<Vertex2D> PrimitiveRenderer::vertex_batch;

// --- Built-in GLSL Shaders ---
static const char* vertexSource = R"(
        #version 330 core
        layout (location = 0) in vec2 aPos;
        layout (location = 1) in vec4 aColor;
        layout (location = 2) in vec2 aTexCoord;

        out vec4 vColor;
        out vec2 vTexCoord;

        void main() {
            vColor = aColor;
            vTexCoord = aTexCoord;
            gl_Position = vec4(aPos, 0.0, 1.0);
        }
    )";

static const char* fragmentSource = R"(
        #version 330 core
        in vec4 vColor;
        in vec2 vTexCoord;
        out vec4 FragColor;

        void main() {
            FragColor = vColor;
        }
    )";

void PrimitiveRenderer::Init() {
  // Initialize Buffers using Utility Helpers
  BufferUtils::CreateBasicBuffers(vao, vbo, ebo,
                                  MAX_VERTICES * sizeof(Vertex2D));

  // Define Vertex Layout using Utility Helpers
  // Position: index 0, 2 floats
  BufferUtils::SetAttribute(0, 2, sizeof(Vertex2D),
                            offsetof(Vertex2D, position));
  // Color: index 1, 4 floats
  BufferUtils::SetAttribute(1, 4, sizeof(Vertex2D), offsetof(Vertex2D, color));
  // TexCoords: index 2, 2 floats
  BufferUtils::SetAttribute(2, 2, sizeof(Vertex2D),
                            offsetof(Vertex2D, texCoords));

  // Pre-populate Index Buffer (static pattern: 0,1,2, 2,3,0)
  std::vector<unsigned int> indices(MAX_INDICES);
  unsigned int offset = 0;
  for (size_t i = 0; i < MAX_INDICES; i += 6) {
    indices[i + 0] = offset + 0;
    indices[i + 1] = offset + 1;
    indices[i + 2] = offset + 2;
    indices[i + 3] = offset + 2;
    indices[i + 4] = offset + 3;
    indices[i + 5] = offset + 0;
    offset += 4;
  }
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
               indices.data(), GL_STATIC_DRAW);

  glBindVertexArray(0);

  // Compile Built-in Shaders
  default_shader = std::unique_ptr<Shader>(
      Shader::CreateFromSource(vertexSource, fragmentSource));

  // Reserve CPU Batch Memory
  vertex_batch.reserve(MAX_VERTICES);
}

void PrimitiveRenderer::Shutdown() {
  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo);
  glDeleteBuffers(1, &ebo);
  default_shader.reset();
  vertex_batch.clear();
}

// --- Batch Control ---

void PrimitiveRenderer::StartBatch() { vertex_batch.clear(); }

void PrimitiveRenderer::FinalizeBatch() {
  if (vertex_batch.empty()) return;

  // Upload CPU data to GPU dynamic buffer
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferSubData(GL_ARRAY_BUFFER, 0, vertex_batch.size() * sizeof(Vertex2D),
                  vertex_batch.data());
}

void PrimitiveRenderer::RenderBatch() {
  if (vertex_batch.empty()) return;

  default_shader->Bind();
  glBindVertexArray(vao);

  // 6 indices per 4 vertices (1 quad)
  uint32_t indexCount = static_cast<uint32_t>((vertex_batch.size() / 4) * 6);
  glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);

  glBindVertexArray(0);
  default_shader->Unbind();
}

// --- Submission API ---

void PrimitiveRenderer::SubmitQuad(float x, float y, float w, float h,
                                   const float color[4]) {
  // Handle batch overflow
  if (vertex_batch.size() + 4 > MAX_VERTICES) {
    FinalizeBatch();
    RenderBatch();
    StartBatch();
  }

  // Add vertices for a single quad (Counter-Clockwise)
  // Bottom-Left
  vertex_batch.push_back(
      {{x, y}, {color[0], color[1], color[2], color[3]}, {0.0f, 0.0f}});
  // Bottom-Right
  vertex_batch.push_back(
      {{x + w, y}, {color[0], color[1], color[2], color[3]}, {1.0f, 0.0f}});
  // Top-Right
  vertex_batch.push_back(
      {{x + w, y + h}, {color[0], color[1], color[2], color[3]}, {1.0f, 1.0f}});
  // Top-Left
  vertex_batch.push_back(
      {{x, y + h}, {color[0], color[1], color[2], color[3]}, {0.0f, 1.0f}});
}

}  // namespace engine::graphics