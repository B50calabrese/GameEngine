#ifndef PRIMITIVE_RENDERER_H
#define PRIMITIVE_RENDERER_H

#include <graphics/shader.h>
#include <graphics/vertex2d.h>

#include <array>
#include <cstdint>
#include <glm/mat4x4.hpp>
#include <memory>
#include <vector>

namespace engine::graphics {

/**
 * Handles basic drawing of 'primitives' or geometric 2D objects.
 */
class PrimitiveRenderer {
 public:
  /**
   * Initializes the renderer to be ready for use.
   */
  static void Init();
  /**
   * Shuts down the renderer and frees any allocated resources.
   */
  static void Shutdown();

  /**
   * Starts a batch for submitting primitives to be drawn (called at the
   * beginning of a frame).
   */
  static void StartBatch(const glm::mat4& view_projection);

  /**
   * 'Finishes' the batch of primitives to be drawn preparing the data for
   * rendering.
   */
  static void FinalizeBatch();

  /**
   * Makes the actual GL calls to render the submitted primitives to the screen.
   */
  static void RenderBatch();

  static int GetTextureSlot(unsigned int texture_id);

  /**
   * Submits a colored rectangle (quad) to be drawn at the specified position
   * and size.
   *
   * @param x The x-coordinate of the rectangle's top-left corner.
   * @param y The y-coordinate of the rectangle's top-left corner.
   * @param w The width of the rectangle.
   * @param h The height of the rectangle.
   * @param color An array of four floats representing the RGBA color of the
   * rectangle.
   */
  static void SubmitQuad(float x, float y, float w, float h,
                         const float color[4]);

  /**
   * Submits a textured rectangle (quad) to be drawn at the specified position
   * and size.
   *
   * @param x The x-coordinate of the rectangle's top-left corner.
   * @param y The y-coordinate of the rectangle's top-left corner.
   * @param w The width of the rectangle.
   * @param h The height of the rectangle.
   * @param texture_id The OpenGL texture ID to use.
   * @param color An array of four floats representing the RGBA color of the
   * rectangle.
   */
  static void SubmitTexturedQuad(float x, float y, float w, float h,
                                 unsigned int texture_id, const float color[4]);

 private:
  // OpenGL buffers.
  static unsigned int vao, vbo, ebo;

  // Default Shader used for rendering
  static std::unique_ptr<Shader> default_shader;

  // Batch of vertices to draw.
  static std::vector<Vertex2D> vertex_batch;

  // Texture Batching State
  static std::array<unsigned int, 32> texture_slots;
  static uint32_t texture_slot_index;

  static constexpr size_t MAX_QUADS = 1000;
  static constexpr size_t MAX_VERTICES = MAX_QUADS * 4;
  static constexpr size_t MAX_INDICES = MAX_QUADS * 6;
};

}  // namespace engine::graphics

#endif  // PRIMITIVE_RENDERER_H