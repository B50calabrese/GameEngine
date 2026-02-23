#include "primitive_renderer.h"

// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

#include <array>
#include <cstddef>
#include <cstdint>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <memory>
#include <vector>

#include "buffer_utils.h"
#include "graphics/shader.h"
#include "vertex2d.h"

namespace engine::graphics {

unsigned int PrimitiveRenderer::vao_ = 0;
unsigned int PrimitiveRenderer::vbo_ = 0;
unsigned int PrimitiveRenderer::ebo_ = 0;
std::unique_ptr<Shader> PrimitiveRenderer::default_shader_ = nullptr;
std::vector<Vertex2D> PrimitiveRenderer::vertex_batch_;
std::array<unsigned int, 32> PrimitiveRenderer::texture_slots_;
uint32_t PrimitiveRenderer::texture_slot_index_ =
    1;  // Slot 0 is reserved for the white texture
glm::mat4 PrimitiveRenderer::current_view_projection_ = glm::mat4(1.0f);

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
  BufferUtils::CreateBasicBuffers(vao_, vbo_, ebo_,
                                  kMaxVertices * sizeof(Vertex2D));

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
  std::vector<unsigned int> indices(kMaxIndices);
  unsigned int offset = 0;
  for (size_t i = 0; i < kMaxIndices; i += 6) {
    indices[i + 0] = offset + 0;
    indices[i + 1] = offset + 1;
    indices[i + 2] = offset + 2;
    indices[i + 3] = offset + 2;
    indices[i + 4] = offset + 3;
    indices[i + 5] = offset + 0;
    offset += 4;
  }
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
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
  texture_slots_[0] = whiteTex;

  // Compile Built-in Shaders
  default_shader_ = std::unique_ptr<Shader>(
      Shader::CreateFromSource(vertexSource, fragmentSource));

  default_shader_->Bind();
  int samplers[32];
  for (int i = 0; i < 32; i++) samplers[i] = i;

  int location = glGetUniformLocation(default_shader_->id(), "uTextures");
  glUniform1iv(location, 32, samplers);

  // Reserve CPU Batch Memory
  vertex_batch_.reserve(kMaxVertices);
}

void PrimitiveRenderer::Shutdown() {
  glDeleteVertexArrays(1, &vao_);
  glDeleteBuffers(1, &vbo_);
  glDeleteBuffers(1, &ebo_);
  glDeleteTextures(1, &texture_slots_[0]);
  default_shader_.reset();
  vertex_batch_.clear();
}

// --- Batch Control ---

void PrimitiveRenderer::StartBatch(const glm::mat4& view_projection) {
  current_view_projection_ = view_projection;
  vertex_batch_.clear();
  texture_slot_index_ = 1;  // Reset to 1, preserving the white texture at 0
  default_shader_->Bind();
  default_shader_->SetMat4("u_ViewProjection", view_projection);
}

void PrimitiveRenderer::FinalizeBatch() {
  if (vertex_batch_.empty()) return;

  // Upload CPU data to GPU dynamic buffer
  glBindBuffer(GL_ARRAY_BUFFER, vbo_);
  glBufferSubData(GL_ARRAY_BUFFER, 0, vertex_batch_.size() * sizeof(Vertex2D),
                  vertex_batch_.data());
}

void PrimitiveRenderer::RenderBatch() {
  if (vertex_batch_.empty()) return;

  default_shader_->Bind();

  // Bind all textures currently active in this batch
  for (uint32_t i = 0; i < texture_slot_index_; i++) {
    glActiveTexture(GL_TEXTURE0 + i);
    glBindTexture(GL_TEXTURE_2D, texture_slots_[i]);
  }

  glBindVertexArray(vao_);

  // 6 indices per 4 vertices (1 quad)
  uint32_t indexCount = static_cast<uint32_t>((vertex_batch_.size() / 4) * 6);
  glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);

  glBindVertexArray(0);
  default_shader_->Unbind();
}

int PrimitiveRenderer::GetTextureSlot(unsigned int texture_id) {
  // Search if texture is already in a slot
  for (uint32_t i = 0; i < texture_slot_index_; i++) {
    if (texture_slots_[i] == texture_id) return (int)i;
  }

  texture_slots_[texture_slot_index_] = texture_id;
  return (int)texture_slot_index_++;
}

// --- Submission API ---

void PrimitiveRenderer::SubmitQuad(float x, float y, float w, float h,
                                   const float color[4]) {
  // Handle batch overflow
  if (vertex_batch_.size() + 4 > kMaxVertices) {
    RenderBatch();
    StartBatch(current_view_projection_);
  }

  float texIndex = 0.0f;  // Use the white texture
  // Add vertices for a single quad (Counter-Clockwise)
  // Bottom-Left
  vertex_batch_.push_back({{x, y},
                           {color[0], color[1], color[2], color[3]},
                           {0.0f, 0.0f},
                           texIndex});
  // Bottom-Right
  vertex_batch_.push_back({{x + w, y},
                           {color[0], color[1], color[2], color[3]},
                           {1.0f, 0.0f},
                           texIndex});
  // Top-Right
  vertex_batch_.push_back({{x + w, y + h},
                           {color[0], color[1], color[2], color[3]},
                           {1.0f, 1.0f},
                           texIndex});
  // Top-Left
  vertex_batch_.push_back({{x, y + h},
                           {color[0], color[1], color[2], color[3]},
                           {0.0f, 1.0f},
                           texIndex});
}

void PrimitiveRenderer::SubmitTexturedQuad(float x, float y, float w, float h,
                                           unsigned int texture_id,
                                           const float color[4], float rotation,
                                           bool flip_uv) {
  // Handle batch overflow or when we have too many textures.
  if (vertex_batch_.size() + 4 > kMaxVertices || texture_slot_index_ >= 32) {
    RenderBatch();
    StartBatch(current_view_projection_);
  }

  float tex_index = (float)GetTextureSlot(texture_id);

  // 1. Define local vertices.
  glm::vec4 local_vertices[4] = {
      {0.0f, 0.0f, 0.0f, 1.0f},  // Top-Left
      {w, 0.0f, 0.0f, 1.0f},     // Top-Right
      {w, h, 0.0f, 1.0f},        // Bottom-Right
      {0.0f, h, 0.0f, 1.0f}      // Bottom-Left
  };

  // 2. Apply Rotation to the local positions
  if (rotation != 0.0f) {
    glm::mat4 rotation_matrix = glm::rotate(
        glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0, 0, 1));

    for (int i = 0; i < 4; i++) {
      local_vertices[i] = rotation_matrix * local_vertices[i];
    }
  }

  // 3. Set UVs based on flip_uv
  float u0 = 0.0f, v0 = 0.0f, u1 = 1.0f, v1 = 1.0f;
  if (flip_uv) {
    v0 = 1.0f;  // Top becomes 1.0
    v1 = 0.0f;  // Bottom becomes 0.0
  }

  float texture_coords[4][2] = {
      {u0, v0},  // Top-Left
      {u1, v0},  // Top-Right
      {u1, v1},  // Bottom-Right
      {u0, v1}   // Bottom-Left
  };

  // 4. Submit vertices to batch
  // We add the world (x, y) translation to the rotated local vertices.
  for (int i = 0; i < 4; i++) {
    Vertex2D v;
    v.position[0] = x + local_vertices[i].x;
    v.position[1] = y + local_vertices[i].y;

    v.color[0] = color[0];
    v.color[1] = color[1];
    v.color[2] = color[2];
    v.color[3] = color[3];

    v.texCoords[0] = texture_coords[i][0];
    v.texCoords[1] = texture_coords[i][1];
    v.texIndex = tex_index;

    vertex_batch_.push_back(v);
  }
}

}  // namespace engine::graphics
