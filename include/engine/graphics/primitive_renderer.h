/**
 * @file primitive_renderer.h
 * @brief Batch rendering for geometry.
 */

#ifndef SRC_ENGINE_GRAPHICS_PRIMITIVE_RENDERER_H_
#define SRC_ENGINE_GRAPHICS_PRIMITIVE_RENDERER_H_

#include <array>
#include <cstdint>
#include <memory>
#include <vector>

#include <glm/mat4x4.hpp>

#include <engine/graphics/shader.h>
#include <engine/graphics/vertex2d.h>

namespace engine::graphics {

/**
 * @brief Handles basic drawing of 'primitives' or geometric 2D objects.
 */
class PrimitiveRenderer {
 public:
  /** @brief Initializes the renderer to be ready for use. */
  static void Init();
  /** @brief Shuts down the renderer and frees any allocated resources. */
  static void Shutdown();

  /**
   * @brief Starts a batch for submitting primitives to be drawn (called at the
   * beginning of a frame).
   * @param view_projection The view-projection matrix for the batch.
   */
  static void StartBatch(const glm::mat4& view_projection);

  /**
   * @brief 'Finishes' the batch of primitives to be drawn preparing the data
   * for rendering.
   */
  static void FinalizeBatch();

  /**
   * @brief Makes the actual GL calls to render the submitted primitives to the
   * screen.
   */
  static void RenderBatch();

  /**
   * @brief Gets the texture slot for a given texture ID.
   * @param texture_id The OpenGL texture ID.
   * @return The slot index.
   */
  static int GetTextureSlot(unsigned int texture_id);

  /**
   * @brief Submits a colored rectangle (quad) to be drawn at the specified
   * position and size.
   */
  static void SubmitQuad(const glm::vec2& position, const glm::vec2& size,
                         const glm::vec4& color, float rotation = 0.0f,
                         const glm::vec2& origin = {0.0f, 0.0f});

  /**
   * @brief Submits a textured rectangle (quad) to be drawn at the specified
   * position and size.
   */
  static void SubmitTexturedQuad(const glm::vec2& position,
                                 const glm::vec2& size, unsigned int texture_id,
                                 const glm::vec4& color, float rotation = 0.0f,
                                 const glm::vec2& origin = {0.0f, 0.0f},
                                 bool flip_uv = false);

  /**
   * @brief Submits a textured rectangle (quad) with custom UV coordinates and
   * styles.
   */
  static void SubmitTexturedQuad(
      const glm::vec2& position, const glm::vec2& size, unsigned int texture_id,
      const glm::vec2& uv_min, const glm::vec2& uv_max, const glm::vec4& color,
      float rotation = 0.0f, const glm::vec2& origin = {0.0f, 0.0f},
      bool is_font = false, float thickness = 0.0f, float roundness = 0.0f,
      const glm::vec4& color2 = glm::vec4(1.0f), int gradient_type = 0);

  /**
   * @brief Submits a circle to be drawn.
   */
  static void SubmitCircle(const glm::vec2& position, float radius,
                           const glm::vec4& color, float thickness = 0.0f,
                           const glm::vec4& color2 = glm::vec4(1.0f),
                           int gradient_type = 0);

  /**
   * @brief Submits a triangle to be drawn.
   */
  static void SubmitTriangle(const glm::vec2& position, const glm::vec2& size,
                             const glm::vec4& color, float rotation = 0.0f,
                             const glm::vec2& origin = {0.0f, 0.0f},
                             float thickness = 0.0f,
                             const glm::vec4& color2 = glm::vec4(1.0f),
                             int gradient_type = 0);

  /**
   * @brief Submits a line to be drawn between two points.
   */
  static void SubmitLine(const glm::vec2& start, const glm::vec2& end,
                         const glm::vec4& color, float thickness = 1.0f,
                         bool is_dashed = false);

  /**
   * @brief Submits a single point to be drawn.
   */
  static void SubmitPoint(const glm::vec2& position, const glm::vec4& color,
                          float size = 1.0f);

  /**
   * @brief Submits a convex polygon to be drawn.
   * @param vertices List of vertices in world space.
   * @param color Fill color.
   */
  static void SubmitPolygon(const std::vector<glm::vec2>& vertices,
                            const glm::vec4& color);

 private:
  // OpenGL buffers.
  static unsigned int vao_, vbo_, ebo_;

  // Default Shader used for rendering
  static std::shared_ptr<Shader> default_shader_;

  // Batch of vertices to draw.
  static std::vector<Vertex2D> vertex_batch_;

  // Texture Batching State
  static std::array<unsigned int, 32> texture_slots_;
  static uint32_t texture_slot_index_;

  // Cached view matrix at the start of a batch.
  static glm::mat4 current_view_projection_;

  static constexpr size_t kMaxQuads = 1000;
  static constexpr size_t kMaxVertices = kMaxQuads * 4;
  static constexpr size_t kMaxIndices = kMaxQuads * 6;
};

}  // namespace engine::graphics

#endif  // SRC_ENGINE_GRAPHICS_PRIMITIVE_RENDERER_H_
