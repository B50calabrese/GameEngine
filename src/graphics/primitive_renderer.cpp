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
            vec4 texColor;
            int index = int(vTexIndex);
            switch(index) {
                case 0:  texColor = texture(uTextures[0], vTexCoord); break;
                case 1:  texColor = texture(uTextures[1], vTexCoord); break;
                case 2:  texColor = texture(uTextures[2], vTexCoord); break;
                case 3:  texColor = texture(uTextures[3], vTexCoord); break;
                case 4:  texColor = texture(uTextures[4], vTexCoord); break;
                case 5:  texColor = texture(uTextures[5], vTexCoord); break;
                case 6:  texColor = texture(uTextures[6], vTexCoord); break;
                case 7:  texColor = texture(uTextures[7], vTexCoord); break;
                case 8:  texColor = texture(uTextures[8], vTexCoord); break;
                case 9:  texColor = texture(uTextures[9], vTexCoord); break;
                case 10: texColor = texture(uTextures[10], vTexCoord); break;
                case 11: texColor = texture(uTextures[11], vTexCoord); break;
                case 12: texColor = texture(uTextures[12], vTexCoord); break;
                case 13: texColor = texture(uTextures[13], vTexCoord); break;
                case 14: texColor = texture(uTextures[14], vTexCoord); break;
                case 15: texColor = texture(uTextures[15], vTexCoord); break;
                case 16: texColor = texture(uTextures[16], vTexCoord); break;
                case 17: texColor = texture(uTextures[17], vTexCoord); break;
                case 18: texColor = texture(uTextures[18], vTexCoord); break;
                case 19: texColor = texture(uTextures[19], vTexCoord); break;
                case 20: texColor = texture(uTextures[20], vTexCoord); break;
                case 21: texColor = texture(uTextures[21], vTexCoord); break;
                case 22: texColor = texture(uTextures[22], vTexCoord); break;
                case 23: texColor = texture(uTextures[23], vTexCoord); break;
                case 24: texColor = texture(uTextures[24], vTexCoord); break;
                case 25: texColor = texture(uTextures[25], vTexCoord); break;
                case 26: texColor = texture(uTextures[26], vTexCoord); break;
                case 27: texColor = texture(uTextures[27], vTexCoord); break;
                case 28: texColor = texture(uTextures[28], vTexCoord); break;
                case 29: texColor = texture(uTextures[29], vTexCoord); break;
                case 30: texColor = texture(uTextures[30], vTexCoord); break;
                case 31: texColor = texture(uTextures[31], vTexCoord); break;
                default: texColor = vec4(1.0); break;
            }
            color = texColor * vColor;
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
                             offsetof(Vertex2D, tex_coords));
  // TexIndex: index 3, 1 float
  BufferUtils::SetAttribute(3, 1, sizeof(Vertex2D),
                             offsetof(Vertex2D, tex_index));

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

  if (default_shader_) {
    default_shader_->Bind();
    int samplers[32];
    for (int i = 0; i < 32; i++) samplers[i] = i;

    int location = glGetUniformLocation(default_shader_->id(), "uTextures");
    glUniform1iv(location, 32, samplers);
  }

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
  if (default_shader_) {
    default_shader_->Bind();
    default_shader_->SetMat4("u_ViewProjection", view_projection);
  }
}

void PrimitiveRenderer::FinalizeBatch() {
  if (vertex_batch_.empty()) return;

  // Upload CPU data to GPU dynamic buffer
  glBindBuffer(GL_ARRAY_BUFFER, vbo_);
  glBufferSubData(GL_ARRAY_BUFFER, 0, vertex_batch_.size() * sizeof(Vertex2D),
                  vertex_batch_.data());
}

void PrimitiveRenderer::RenderBatch() {
  if (vertex_batch_.empty() || !default_shader_) return;

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

void PrimitiveRenderer::SubmitQuad(const glm::vec2& position,
                                    const glm::vec2& size,
                                    const glm::vec4& color, float rotation,
                                    const glm::vec2& origin) {
  SubmitTexturedQuad(position, size, texture_slots_[0], color, rotation,
                       origin, false);
}

void PrimitiveRenderer::SubmitTexturedQuad(const glm::vec2& position,
                                             const glm::vec2& size,
                                             unsigned int texture_id,
                                             const glm::vec4& color,
                                             float rotation,
                                             const glm::vec2& origin,
                                             bool flip_uv) {
  // Handle batch overflow or when we have too many textures.
  if (vertex_batch_.size() + 4 > kMaxVertices || texture_slot_index_ >= 32) {
    RenderBatch();
    StartBatch(current_view_projection_);
  }

  float tex_index = (float)GetTextureSlot(texture_id);

  float w = size.x;
  float h = size.y;

  // 1. Define local vertices relative to the origin.
  float offsetX = origin.x * w;
  float offsetY = origin.y * h;

  glm::vec4 local_vertices[4] = {
      {-offsetX, -offsetY, 0.0f, 1.0f},      // Bottom-Left
      {w - offsetX, -offsetY, 0.0f, 1.0f},   // Bottom-Right
      {w - offsetX, h - offsetY, 0.0f, 1.0f},  // Top-Right
      {-offsetX, h - offsetY, 0.0f, 1.0f}     // Top-Left
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
      {u0, v0},  // Bottom-Left
      {u1, v0},  // Bottom-Right
      {u1, v1},  // Top-Right
      {u0, v1}   // Top-Left
  };

  // 4. Submit vertices to batch
  for (int i = 0; i < 4; i++) {
    Vertex2D v;
    v.position[0] = position.x + local_vertices[i].x;
    v.position[1] = position.y + local_vertices[i].y;

    v.color[0] = color.r;
    v.color[1] = color.g;
    v.color[2] = color.b;
    v.color[3] = color.a;

    v.tex_coords[0] = texture_coords[i][0];
    v.tex_coords[1] = texture_coords[i][1];
    v.tex_index = tex_index;

    vertex_batch_.push_back(v);
  }
}

}  // namespace engine::graphics
