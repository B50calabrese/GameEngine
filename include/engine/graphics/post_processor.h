/**
 * @file post_processor.h
 * @brief Manages full-screen post-processing effects.
 */

#ifndef INCLUDE_ENGINE_GRAPHICS_POST_PROCESSOR_H_
#define INCLUDE_ENGINE_GRAPHICS_POST_PROCESSOR_H_

#include <memory>

#include <glm/glm.hpp>

#include <engine/graphics/framebuffer.h>
#include <engine/graphics/shader.h>

namespace engine::graphics {

/**
 * @brief Singleton manager for post-processing effects.
 *
 * Handles the transition between scene rendering and post-process passes.
 */
class PostProcessManager {
 public:
  /**
   * @brief Returns a reference to the singleton `PostProcessManager` instance.
   * @return Reference to the PostProcessManager.
   */
  static PostProcessManager& Get() {
    static PostProcessManager instance;
    return instance;
  }

  /**
   * @brief Initializes the manager with given dimensions.
   * @param width Initial width.
   * @param height Initial height.
   */
  void Init(int width, int height);

  /** @brief Prepares the FBO for scene rendering. */
  void Begin();

  /** @brief Finalizes scene and renders the post-process quad to screen. */
  void End();

  /**
   * @brief Notifies the manager of a window resize.
   * @param width New width.
   * @param height New height.
   */
  void OnResize(int width, int height);

  /**
   * @brief Sets the intensity of the screen shake effect.
   * @param intensity Shake intensity (0.0 to 1.0 recommended).
   */
  void SetShake(float intensity) { shake_intensity_ = intensity; }

  /**
   * @brief Sets the screen flash effect parameters.
   * @param color The color of the flash.
   * @param amount The intensity of the flash (0.0 to 1.0).
   */
  void SetFlash(const glm::vec3& color, float amount) {
    flash_color_ = color;
    flash_amount_ = amount;
  }

  /**
   * @brief Sets the palette texture for color mapping.
   * @param texture_id The OpenGL ID of the palette texture.
   */
  void SetPalette(unsigned int texture_id) { palette_tex_id_ = texture_id; }

 private:
  PostProcessManager() = default;
  ~PostProcessManager();

  // Prevent copy/move
  PostProcessManager(const PostProcessManager&) = delete;
  PostProcessManager& operator=(const PostProcessManager&) = delete;

  /** @brief Initializes the full-screen quad VAO. */
  void InitQuad();

  std::unique_ptr<Framebuffer> scene_buffer_;
  std::shared_ptr<Shader> post_shader_;
  unsigned int quad_vao_ = 0;
  unsigned int quad_vbo_ = 0;

  // Effect Parameters
  float shake_intensity_ = 0.0f;
  glm::vec3 flash_color_ = {1.0f, 1.0f, 1.0f};
  float flash_amount_ = 0.0f;
  unsigned int palette_tex_id_ = 0;

  int width_ = 0;
  int height_ = 0;
};

}  // namespace engine::graphics

#endif  // INCLUDE_ENGINE_GRAPHICS_POST_PROCESSOR_H_
