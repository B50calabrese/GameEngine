#ifndef SRC_GRAPHICS_PRIMITIVE_RENDERER_H_
#define SRC_GRAPHICS_PRIMITIVE_RENDERER_H_

#include <graphics/shader.h>
#include <graphics/vertex2d.h>

#include <array>
#include <cstdint>
#include <glm/mat4x4.hpp>
#include <memory>
#include <vector>

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
   *
   * @param position The position of the rectangle's top-left corner.
   * @param size The size of the rectangle.
   * @param color The RGBA color of the rectangle.
   * @param rotation The amount to rotate the quad in degrees.
   * @param origin The origin point for rotation (relative to size, 0-1).
   */
  static void SubmitQuad(const glm::vec2& position, const glm::vec2& size,
                         const glm::vec4& color, float rotation = 0.0f,
                         const glm::vec2& origin = {0.0f, 0.0f});

  /**
   * @brief Submits a textured rectangle (quad) to be drawn at the specified
   * position and size.
   *
   * @param position The position of the rectangle's top-left corner.
   * @param size The size of the rectangle.
   * @param texture_id The OpenGL texture ID to use.
   * @param color The RGBA color (tint) of the rectangle.
   * @param rotation The amount to rotate the quad in degrees.
   * @param origin The origin point for rotation (relative to size, 0-1).
   * @param flip_uv Whether to flip the way the textured quad is rendered.
   */
  static void SubmitTexturedQuad(const glm::vec2& position,
                                 const glm::vec2& size, unsigned int texture_id,
                                 const glm::vec4& color, float rotation = 0.0f,
                                 const glm::vec2& origin = {0.0f, 0.0f},
                                 bool flip_uv = false);

 private:
  // OpenGL buffers.
  static unsigned int vao_, vbo_, ebo_;

  // Default Shader used for rendering
  static std::unique_ptr<Shader> default_shader_;

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

#endif  // SRC_GRAPHICS_PRIMITIVE_RENDERER_H_
