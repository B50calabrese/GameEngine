/**
 * @file primitive_renderer.h
 * @brief Batch rendering for geometry.
 */

#ifndef INCLUDE_ENGINE_GRAPHICS_PRIMITIVE_RENDERER_H_
#define INCLUDE_ENGINE_GRAPHICS_PRIMITIVE_RENDERER_H_

#include <array>
#include <cstdint>
#include <memory>
#include <vector>

#include <glm/mat4x4.hpp>

#include <engine/graphics/shader.h>
#include <engine/graphics/vertex2d.h>

namespace engine::graphics {

/**
 * @brief Handles basic drawing of geometric 2D objects.
 */
class PrimitiveRenderer {
 public:
  /** @brief Initializes the renderer. */
  static void Init();

  /** @brief Shuts down the renderer. */
  static void Shutdown();

  /** @brief Starts a new batch. */
  static void StartBatch(const glm::mat4& view_projection);

  /** @brief Finalizes the batch data. */
  static void FinalizeBatch();

  /** @brief Renders the current batch. */
  static void RenderBatch();

  /** @brief Gets the texture slot for a texture ID. */
  static int GetTextureSlot(unsigned int texture_id);

  /** @brief Submits a colored quad. */
  static void SubmitQuad(const glm::vec2& position, const glm::vec2& size,
                         const glm::vec4& color, float rotation = 0.0f,
                         const glm::vec2& origin = {0.0f, 0.0f});

  /** @brief Submits a textured quad. */
  static void SubmitTexturedQuad(const glm::vec2& position,
                                 const glm::vec2& size, unsigned int texture_id,
                                 const glm::vec4& color, float rotation = 0.0f,
                                 const glm::vec2& origin = {0.0f, 0.0f},
                                 bool flip_uv = false);

  /** @brief Submits a textured quad with custom UVs. */
  static void SubmitTexturedQuad(const glm::vec2& position,
                                 const glm::vec2& size, unsigned int texture_id,
                                 const glm::vec2& uv_min,
                                 const glm::vec2& uv_max,
                                 const glm::vec4& color, float rotation = 0.0f,
                                 const glm::vec2& origin = {0.0f, 0.0f},
                                 bool is_font = false);

 private:
  static unsigned int vao_, vbo_, ebo_;
  static std::shared_ptr<Shader> default_shader_;
  static std::vector<Vertex2D> vertex_batch_;
  static std::array<unsigned int, 32> texture_slots_;
  static uint32_t texture_slot_index_;
  static glm::mat4 current_view_projection_;

  static constexpr size_t kMaxQuads = 1000;
  static constexpr size_t kMaxVertices = kMaxQuads * 4;
  static constexpr size_t kMaxIndices = kMaxQuads * 6;
};

}  // namespace engine::graphics

#endif  // INCLUDE_ENGINE_GRAPHICS_PRIMITIVE_RENDERER_H_
