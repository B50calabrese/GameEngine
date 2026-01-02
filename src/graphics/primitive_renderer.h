#ifndef PRIMITIVE_RENDERER_H
#define PRIMITIVE_RENDERER_H

#include <graphics/shader.h>

#include <memory>
#include <vector>

namespace engine::graphics {

/**
 * Internal vertex structure for 2D batching.
 */
struct Vertex2D {
  float position[2];
  float color[4];
  float texCoords[2];
};

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
  static void StartBatch();

  /**
   * 'Finishes' the batch of primitives to be drawn preparing the data for
   * rendering.
   */
  static void FinalizeBatch();

  /**
   * Makes the actual GL calls to render the submitted primitives to the screen.
   */
  static void RenderBatch();

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

 private:
  // OpenGL buffers.
  static unsigned int vao, vbo, ebo;

  // Default Shader used for rendering
  static std::unique_ptr<Shader> default_shader;

  // Batch of vertices to draw.
  static std::vector<Vertex2D> vertex_batch;

  static constexpr size_t MAX_QUADS = 1000;
  static constexpr size_t MAX_VERTICES = MAX_QUADS * 4;
  static constexpr size_t MAX_INDICES = MAX_QUADS * 6;
};

}  // namespace engine::graphics

#endif  // PRIMITIVE_RENDERER_H