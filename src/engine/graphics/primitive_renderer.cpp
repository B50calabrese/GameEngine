/**
 * @file primitive_renderer.cpp
 * @brief PrimitiveRenderer implementation.
 */

#include <engine/graphics/primitive_renderer.h>

// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

#include <array>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <vector>

#include <glm/gtc/matrix_transform.hpp>

#include <engine/graphics/buffer_utils.h>
#include <engine/graphics/shader.h>
#include <engine/graphics/vertex2d.h>
#include <engine/util/logger.h>

namespace engine::graphics {

unsigned int PrimitiveRenderer::vao_ = 0;
unsigned int PrimitiveRenderer::vbo_ = 0;
unsigned int PrimitiveRenderer::ebo_ = 0;
std::shared_ptr<Shader> PrimitiveRenderer::default_shader_ = nullptr;
std::vector<Vertex2D> PrimitiveRenderer::vertex_batch_;
std::array<unsigned int, 32> PrimitiveRenderer::texture_slots_;
uint32_t PrimitiveRenderer::texture_slot_index_ =
    1;  // Slot 0 is reserved for the white texture
glm::mat4 PrimitiveRenderer::current_view_projection_ = glm::mat4(1.0f);

// --- Uber Shader Sources (Internal Backup) ---
static const char* kUberVertexSource = R"(
#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec4 aColor2;
layout (location = 3) in vec2 aTexCoord;
layout (location = 4) in vec2 aLocalPos;
layout (location = 5) in float aTexIndex;
layout (location = 6) in float aShapeType;
layout (location = 7) in float aThickness;
layout (location = 8) in float aRoundness;
layout (location = 9) in float aGradientType;
layout (location = 10) in float aIsFont;
layout (location = 11) in float aIsDashed;

uniform mat4 u_ViewProjection;

out vec4 vColor;
out vec4 vColor2;
out vec2 vTexCoord;
out vec2 vLocalPos;
out float vTexIndex;
out float vShapeType;
out float vThickness;
out float vRoundness;
out float vGradientType;
out float vIsFont;
out float vIsDashed;

void main() {
    vColor = aColor;
    vColor2 = aColor2;
    vTexCoord = aTexCoord;
    vLocalPos = aLocalPos;
    vTexIndex = aTexIndex;
    vShapeType = aShapeType;
    vThickness = aThickness;
    vRoundness = aRoundness;
    vGradientType = aGradientType;
    vIsFont = aIsFont;
    vIsDashed = aIsDashed;
    gl_Position = u_ViewProjection * vec4(aPos, 0.0, 1.0);
}
)";

static const char* kUberFragmentSource = R"(
#version 330 core
in vec4 vColor;
in vec4 vColor2;
in vec2 vTexCoord;
in vec2 vLocalPos;
in float vTexIndex;
in float vShapeType;
in float vThickness;
in float vRoundness;
in float vGradientType;
in float vIsFont;
in float vIsDashed;

uniform sampler2D uTextures[32];

out vec4 FragColor;

float sdRoundRect(vec2 p, vec2 b, float r) {
    vec2 d = abs(p) - b + vec2(r);
    return min(max(d.x, d.y), 0.0) + length(max(d, 0.0)) - r;
}

float sdCircle(vec2 p, float r) {
    return length(p) - r;
}

float sdTriangle(vec2 p, float r) {
    const float k = sqrt(3.0);
    p.x = abs(p.x) - r;
    p.y = p.y + r/k;
    if( p.x+k*p.y>0.0 ) p = vec2(p.x-k*p.y,-k*p.x-p.y)/2.0;
    p.x -= clamp( p.x, -2.0*r, 0.0 );
    return -length(p)*sign(p.y);
}

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

    if (vIsFont > 0.5) {
        texColor = vec4(1.0, 1.0, 1.0, texColor.r);
    }

    float dist = 0.0;
    if (vShapeType == 0.0) { // Quad/Rounded Quad
        dist = sdRoundRect(vLocalPos, vec2(1.0, 1.0), vRoundness);
    } else if (vShapeType == 1.0) { // Circle
        dist = sdCircle(vLocalPos, 1.0);
    } else if (vShapeType == 2.0) { // Triangle
        dist = sdTriangle(vLocalPos, 1.0);
    } else if (vShapeType == 3.0) { // Line
        dist = 0.0;
    } else if (vShapeType == 4.0) { // Point
        dist = sdCircle(vLocalPos, 1.0);
    } else if (vShapeType == 5.0) { // Polygon
        dist = 0.0;
    }

    if (dist > 0.0 && vShapeType != 5.0) discard;

    vec4 color1 = vColor;
    vec4 color2 = vColor2;
    vec4 baseColor = color1;

    if (vGradientType == 1.0) { // Linear
        float t = clamp(vLocalPos.y * 0.5 + 0.5, 0.0, 1.0);
        baseColor = mix(color1, color2, t);
    } else if (vGradientType == 2.0) { // Radial
        float t = clamp(length(vLocalPos), 0.0, 1.0);
        baseColor = mix(color1, color2, t);
    }

    vec4 finalColor = baseColor * texColor;

    if (vShapeType == 3.0 && vIsDashed > 0.5) {
        if (mod(vLocalPos.x * 20.0, 2.0) < 1.0) discard;
    }

    if (vThickness > 0.0 && vShapeType != 3.0 && vShapeType != 5.0) {
        float edge = 0.02; // Anti-aliasing
        float alpha = smoothstep(0.0, -edge, dist) - smoothstep(-vThickness, -vThickness - edge, dist);
        finalColor.a *= alpha;
    }

    FragColor = finalColor;
}
)";

void PrimitiveRenderer::Init() {
  BufferUtils::CreateBasicBuffers(vao_, vbo_, ebo_,
                                  kMaxVertices * sizeof(Vertex2D));

  glBindVertexArray(vao_);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_);

  // Layout Setup
  BufferUtils::SetAttribute(0, 2, sizeof(Vertex2D),
                            offsetof(Vertex2D, position));
  BufferUtils::SetAttribute(1, 4, sizeof(Vertex2D), offsetof(Vertex2D, color));
  BufferUtils::SetAttribute(2, 4, sizeof(Vertex2D), offsetof(Vertex2D, color2));
  BufferUtils::SetAttribute(3, 2, sizeof(Vertex2D),
                            offsetof(Vertex2D, tex_coords));
  BufferUtils::SetAttribute(4, 2, sizeof(Vertex2D),
                            offsetof(Vertex2D, local_pos));
  BufferUtils::SetAttribute(5, 1, sizeof(Vertex2D),
                            offsetof(Vertex2D, tex_index));
  BufferUtils::SetAttribute(6, 1, sizeof(Vertex2D),
                            offsetof(Vertex2D, shape_type));
  BufferUtils::SetAttribute(7, 1, sizeof(Vertex2D),
                            offsetof(Vertex2D, thickness));
  BufferUtils::SetAttribute(8, 1, sizeof(Vertex2D),
                            offsetof(Vertex2D, roundness));
  BufferUtils::SetAttribute(9, 1, sizeof(Vertex2D),
                            offsetof(Vertex2D, gradient_type));
  BufferUtils::SetAttribute(10, 1, sizeof(Vertex2D),
                            offsetof(Vertex2D, is_font));
  BufferUtils::SetAttribute(11, 1, sizeof(Vertex2D),
                            offsetof(Vertex2D, is_dashed));

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

  unsigned int whiteTex;
  glGenTextures(1, &whiteTex);
  glBindTexture(GL_TEXTURE_2D, whiteTex);
  uint32_t whiteData = 0xffffffff;
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE,
               &whiteData);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  texture_slots_[0] = whiteTex;

  default_shader_ =
      Shader::CreateFromSource(kUberVertexSource, kUberFragmentSource);

  if (default_shader_) {
    default_shader_->Bind();
    int samplers[32];
    for (int i = 0; i < 32; i++) samplers[i] = i;
    int location = glGetUniformLocation(default_shader_->id(), "uTextures");
    glUniform1iv(location, 32, samplers);
  }

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

void PrimitiveRenderer::StartBatch(const glm::mat4& view_projection) {
  current_view_projection_ = view_projection;
  vertex_batch_.clear();
  texture_slot_index_ = 1;
  if (default_shader_) {
    default_shader_->Bind();
    default_shader_->SetMat4("u_ViewProjection", view_projection);
  }
}

void PrimitiveRenderer::FinalizeBatch() {
  if (vertex_batch_.empty()) return;
  glBindBuffer(GL_ARRAY_BUFFER, vbo_);
  glBufferSubData(GL_ARRAY_BUFFER, 0, vertex_batch_.size() * sizeof(Vertex2D),
                  vertex_batch_.data());
}

void PrimitiveRenderer::RenderBatch() {
  if (vertex_batch_.empty() || !default_shader_) return;
  default_shader_->Bind();
  for (uint32_t i = 0; i < texture_slot_index_; i++) {
    glActiveTexture(GL_TEXTURE0 + i);
    glBindTexture(GL_TEXTURE_2D, texture_slots_[i]);
  }
  glBindVertexArray(vao_);
  uint32_t index_count = static_cast<uint32_t>((vertex_batch_.size() / 4) * 6);
  // Special case for polygons which might not be multiples of 4 vertices
  // Actually our fan-based submission for polygons also uses triangles
  glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, nullptr);
  glBindVertexArray(0);
  default_shader_->Unbind();
  vertex_batch_.clear();
}

int PrimitiveRenderer::GetTextureSlot(unsigned int texture_id) {
  if (texture_id == 0) return 0;
  for (uint32_t i = 0; i < texture_slot_index_; i++) {
    if (texture_slots_[i] == texture_id) return static_cast<int>(i);
  }
  if (texture_slot_index_ >= 32) {
    FinalizeBatch();
    RenderBatch();
    StartBatch(current_view_projection_);
  }
  texture_slots_[texture_slot_index_] = texture_id;
  return static_cast<int>(texture_slot_index_++);
}

// --- Submission API ---

void PrimitiveRenderer::SubmitQuad(const glm::vec2& position,
                                   const glm::vec2& size,
                                   const glm::vec4& color, float rotation,
                                   const glm::vec2& origin) {
  SubmitTexturedQuad(position, size, 0, color, rotation, origin, false);
}

void PrimitiveRenderer::SubmitTexturedQuad(
    const glm::vec2& position, const glm::vec2& size, unsigned int texture_id,
    const glm::vec4& color, float rotation, const glm::vec2& origin,
    bool flip_uv) {
  glm::vec2 uv_min(0.0f, 0.0f);
  glm::vec2 uv_max(1.0f, 1.0f);
  if (flip_uv) {
    uv_min.y = 1.0f;
    uv_max.y = 0.0f;
  }
  SubmitTexturedQuad(position, size, texture_id, uv_min, uv_max, color,
                     rotation, origin, false);
}

void PrimitiveRenderer::SubmitTexturedQuad(
    const glm::vec2& position, const glm::vec2& size, unsigned int texture_id,
    const glm::vec2& uv_min, const glm::vec2& uv_max, const glm::vec4& color,
    float rotation, const glm::vec2& origin, bool is_font, float thickness,
    float roundness, const glm::vec4& color2, int gradient_type) {
  if (vertex_batch_.size() + 4 > kMaxVertices) {
    FinalizeBatch();
    RenderBatch();
    StartBatch(current_view_projection_);
  }

  float tex_index = static_cast<float>(GetTextureSlot(texture_id));
  float w = size.x;
  float h = size.y;
  float offset_x = origin.x * w;
  float offset_y = origin.y * h;

  glm::vec4 local_vertices[4] = {{-offset_x, -offset_y, 0.0f, 1.0f},
                                 {w - offset_x, -offset_y, 0.0f, 1.0f},
                                 {w - offset_x, h - offset_y, 0.0f, 1.0f},
                                 {-offset_x, h - offset_y, 0.0f, 1.0f}};

  glm::vec2 local_coords[4] = {
      {-1.0f, -1.0f}, {1.0f, -1.0f}, {1.0f, 1.0f}, {-1.0f, 1.0f}};

  if (rotation != 0.0f) {
    glm::mat4 rot = glm::rotate(glm::mat4(1.0f), glm::radians(rotation),
                                glm::vec3(0, 0, 1));
    for (int i = 0; i < 4; i++) local_vertices[i] = rot * local_vertices[i];
  }

  float uvs[4][2] = {{uv_min.x, uv_min.y},
                     {uv_max.x, uv_min.y},
                     {uv_max.x, uv_max.y},
                     {uv_min.x, uv_max.y}};

  for (int i = 0; i < 4; i++) {
    Vertex2D v;
    v.position[0] = position.x + local_vertices[i].x;
    v.position[1] = position.y + local_vertices[i].y;
    v.color[0] = color.r;
    v.color[1] = color.g;
    v.color[2] = color.b;
    v.color[3] = color.a;
    v.color2[0] = color2.r;
    v.color2[1] = color2.g;
    v.color2[2] = color2.b;
    v.color2[3] = color2.a;
    v.tex_coords[0] = uvs[i][0];
    v.tex_coords[1] = uvs[i][1];
    v.local_pos[0] = local_coords[i].x;
    v.local_pos[1] = local_coords[i].y;
    v.tex_index = tex_index;
    v.shape_type = 0.0f;
    v.thickness = thickness;
    v.roundness = roundness;
    v.gradient_type = static_cast<float>(gradient_type);
    v.is_font = is_font ? 1.0f : 0.0f;
    v.is_dashed = 0.0f;
    vertex_batch_.push_back(v);
  }
}

void PrimitiveRenderer::SubmitCircle(const glm::vec2& position, float radius,
                                     const glm::vec4& color, float thickness,
                                     const glm::vec4& color2,
                                     int gradient_type) {
  SubmitTexturedQuad(position - glm::vec2(radius), glm::vec2(radius * 2.0f), 0,
                     {0, 0}, {1, 1}, color, 0.0f, {0, 0}, false, thickness,
                     0.0f, color2, gradient_type);
  for (int i = 0; i < 4; i++) {
    vertex_batch_[vertex_batch_.size() - 1 - i].shape_type = 1.0f;
  }
}

void PrimitiveRenderer::SubmitTriangle(const glm::vec2& position,
                                       const glm::vec2& size,
                                       const glm::vec4& color, float rotation,
                                       const glm::vec2& origin, float thickness,
                                       const glm::vec4& color2,
                                       int gradient_type) {
  SubmitTexturedQuad(position, size, 0, {0, 0}, {1, 1}, color, rotation, origin,
                     false, thickness, 0.0f, color2, gradient_type);
  for (int i = 0; i < 4; i++) {
    vertex_batch_[vertex_batch_.size() - 1 - i].shape_type = 2.0f;
  }
}

void PrimitiveRenderer::SubmitLine(const glm::vec2& start, const glm::vec2& end,
                                   const glm::vec4& color, float thickness,
                                   bool is_dashed) {
  glm::vec2 dir = end - start;
  float length = glm::length(dir);
  float angle = glm::degrees(atan2(dir.y, dir.x));
  SubmitTexturedQuad(start, {length, thickness}, 0, {0, 0}, {1, 1}, color,
                     angle, {0.0f, 0.5f}, false, 0.0f, 0.0f);
  for (int i = 0; i < 4; i++) {
    vertex_batch_[vertex_batch_.size() - 1 - i].shape_type = 3.0f;
    vertex_batch_[vertex_batch_.size() - 1 - i].is_dashed =
        is_dashed ? 1.0f : 0.0f;
  }
}

void PrimitiveRenderer::SubmitPoint(const glm::vec2& position,
                                    const glm::vec4& color, float size) {
  SubmitCircle(position, size * 0.5f, color);
  for (int i = 0; i < 4; i++) {
    vertex_batch_[vertex_batch_.size() - 1 - i].shape_type = 4.0f;
  }
}

void PrimitiveRenderer::SubmitPolygon(const std::vector<glm::vec2>& vertices,
                                      const glm::vec4& color) {
  if (vertices.size() < 3) return;
  // Submit as triangle fan
  for (size_t i = 1; i < vertices.size() - 1; i++) {
    if (vertex_batch_.size() + 4 > kMaxVertices) {
      FinalizeBatch();
      RenderBatch();
      StartBatch(current_view_projection_);
    }
    glm::vec2 p0 = vertices[0];
    glm::vec2 p1 = vertices[i];
    glm::vec2 p2 = vertices[i + 1];

    // We must submit 4 vertices for our index buffer to work properly
    // or we change our index buffer. Since we use a fixed pattern of 6 indices
    // per 4 vertices, we submit a degenerate quad.
    glm::vec2 quad[4] = {p0, p1, p2, p2};
    for (int j = 0; j < 4; j++) {
      Vertex2D v;
      v.position[0] = quad[j].x;
      v.position[1] = quad[j].y;
      v.color[0] = color.r;
      v.color[1] = color.g;
      v.color[2] = color.b;
      v.color[3] = color.a;
      v.color2[0] = 0;
      v.color2[1] = 0;
      v.color2[2] = 0;
      v.color2[3] = 0;
      v.tex_coords[0] = 0;
      v.tex_coords[1] = 0;
      v.local_pos[0] = 0;
      v.local_pos[1] = 0;
      v.tex_index = 0.0f;
      v.shape_type = 5.0f;
      v.thickness = 0.0f;
      v.roundness = 0.0f;
      v.gradient_type = 0.0f;
      v.is_font = 0.0f;
      v.is_dashed = 0.0f;
      vertex_batch_.push_back(v);
    }
  }
}

}  // namespace engine::graphics
