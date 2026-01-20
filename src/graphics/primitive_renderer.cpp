#include "primitive_renderer.h"

#include <glad/glad.h>

#include <array>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <memory>
#include <vector>

#include "buffer_utils.h"
#include "graphics/shader.h"
#include "vertex2d.h"

namespace engine::graphics {

unsigned int PrimitiveRenderer::vao = 0;
unsigned int PrimitiveRenderer::vbo = 0;
unsigned int PrimitiveRenderer::ebo = 0;
std::unique_ptr<Shader> PrimitiveRenderer::default_shader = nullptr;
std::vector<Vertex2D> PrimitiveRenderer::vertex_batch;
std::array<unsigned int, 32> PrimitiveRenderer::texture_slots;
uint32_t PrimitiveRenderer::texture_slot_index =
    1;  // Slot 0 is reserved for the white texture

// --- Built-in GLSL Shaders ---
static const char* vertexSource = R"(
        #version 330 core
        layout (location = 0) in vec2 aPos;
        layout (location = 1) in vec4 aColor;
        layout (location = 2) in vec2 aTexCoord;
        layout (location = 3) in float aTexIndex;

        uniform mat4 u_ViewProjection; // Maps world space to screen space

        out vec4 vColor;
        out vec2 vTexCoord;
        out float vTexIndex;

        void main() {
            vColor = aColor;
            vTexCoord = aTexCoord;
            vTexIndex = aTexIndex;
            // Apply the camera transformation
            gl_Position = u_ViewProjection * vec4(aPos, 0.0, 1.0);
        }
    )";

static const char* fragmentSource = R"(
        #version 330 core
        in vec4 vColor;
        in vec2 vTexCoord;
        in float vTexIndex;

        uniform sampler2D uTextures[32];

        out vec4 color;

        void main() {
            int index = int(vTexIndex);
            // Sample the texture and multiply by the per-vertex color/tint
            color = texture(uTextures[index], vTexCoord) * vColor;
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
  // TexIndex: index 3, 1 float
  BufferUtils::SetAttribute(3, 1, sizeof(Vertex2D),
                            offsetof(Vertex2D, texIndex));

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

  // Create 1x1 White Texture for Slot 0
  unsigned int whiteTex;
  glGenTextures(1, &whiteTex);
  glBindTexture(GL_TEXTURE_2D, whiteTex);
  uint32_t whiteData = 0xffffffff;
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE,
               &whiteData);
  texture_slots[0] = whiteTex;

  // Compile Built-in Shaders
  default_shader = std::unique_ptr<Shader>(
      Shader::CreateFromSource(vertexSource, fragmentSource));

  default_shader->Bind();
  int samplers[32];
  for (int i = 0; i < 32; i++) samplers[i] = i;

  int location = glGetUniformLocation(default_shader->GetId(), "uTextures");
  glUniform1iv(location, 32, samplers);

  // Reserve CPU Batch Memory
  vertex_batch.reserve(MAX_VERTICES);
}

void PrimitiveRenderer::Shutdown() {
  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo);
  glDeleteBuffers(1, &ebo);
  glDeleteTextures(1, &texture_slots[0]);
  default_shader.reset();
  vertex_batch.clear();
}

// --- Batch Control ---

void PrimitiveRenderer::StartBatch(const glm::mat4& view_projection) {
  vertex_batch.clear();
  texture_slot_index = 1;  // Reset to 1, preserving the white texture at 0
  default_shader->Bind();
  default_shader->SetMat4("u_ViewProjection", view_projection);
}

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

  // Bind all textures currently active in this batch
  for (uint32_t i = 0; i < texture_slot_index; i++) {
    glActiveTexture(GL_TEXTURE0 + i);
    glBindTexture(GL_TEXTURE_2D, texture_slots[i]);
  }

  glBindVertexArray(vao);

  // 6 indices per 4 vertices (1 quad)
  uint32_t indexCount = static_cast<uint32_t>((vertex_batch.size() / 4) * 6);
  glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);

  glBindVertexArray(0);
  default_shader->Unbind();
}

int PrimitiveRenderer::GetTextureSlot(unsigned int texture_id) {
  // Search if texture is already in a slot
  for (uint32_t i = 0; i < texture_slot_index; i++) {
    if (texture_slots[i] == texture_id) return (int)i;
  }

  // If slots are full, flush the batch and start fresh
  if (texture_slot_index >= 32) {
    // For now we return 0 to avoid issues with the view matrix.
    return 0;
  }

  texture_slots[texture_slot_index] = texture_id;
  return (int)texture_slot_index++;
}

// --- Submission API ---

void PrimitiveRenderer::SubmitQuad(float x, float y, float w, float h,
                                   const float color[4]) {
  // Handle batch overflow
  if (vertex_batch.size() + 4 > MAX_VERTICES) {
    // In a real engine, we would flush and start a new batch here
    std::cout << "[PrimitiveRenderer] Max quads reached!\n";
    return;
  }

  float texIndex = 0.0f;  // Use the white texture
  // Add vertices for a single quad (Counter-Clockwise)
  // Bottom-Left
  vertex_batch.push_back({{x, y},
                          {color[0], color[1], color[2], color[3]},
                          {0.0f, 0.0f},
                          texIndex});
  // Bottom-Right
  vertex_batch.push_back({{x + w, y},
                          {color[0], color[1], color[2], color[3]},
                          {1.0f, 0.0f},
                          texIndex});
  // Top-Right
  vertex_batch.push_back({{x + w, y + h},
                          {color[0], color[1], color[2], color[3]},
                          {1.0f, 1.0f},
                          texIndex});
  // Top-Left
  vertex_batch.push_back({{x, y + h},
                          {color[0], color[1], color[2], color[3]},
                          {0.0f, 1.0f},
                          texIndex});
}

void PrimitiveRenderer::SubmitTexturedQuad(float x, float y, float w, float h,
                                           unsigned int texture_id,
                                           const float color[4]) {
  // Handle batch overflow
  if (vertex_batch.size() + 4 > MAX_VERTICES) {
    // In a real engine, we would flush and start a new batch here
    std::cout << "[PrimitiveRenderer] Max quads reached!\n";
    return;
  }

  float texIndex = (float)GetTextureSlot(texture_id);

  // Add vertices for a single quad (Counter-Clockwise)
  // Bottom-Left
  vertex_batch.push_back({{x, y},
                          {color[0], color[1], color[2], color[3]},
                          {0.0f, 0.0f},
                          texIndex});
  // Bottom-Right
  vertex_batch.push_back({{x + w, y},
                          {color[0], color[1], color[2], color[3]},
                          {1.0f, 0.0f},
                          texIndex});
  // Top-Right
  vertex_batch.push_back({{x + w, y + h},
                          {color[0], color[1], color[2], color[3]},
                          {1.0f, 1.0f},
                          texIndex});
  // Top-Left
  vertex_batch.push_back({{x, y + h},
                          {color[0], color[1], color[2], color[3]},
                          {0.0f, 1.0f},
                          texIndex});
}

}  // namespace engine::graphics