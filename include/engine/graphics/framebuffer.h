/**
 * @file framebuffer.h
 * @brief Framebuffer wrapper for off-screen rendering.
 */

#ifndef INCLUDE_ENGINE_GRAPHICS_FRAMEBUFFER_H_
#define INCLUDE_ENGINE_GRAPHICS_FRAMEBUFFER_H_

#include <glad/glad.h>

/**
 * @namespace engine::graphics
 * @brief Graphics and rendering systems.
 */
namespace engine::graphics {

/**
 * @brief Manages an off-screen render target.
 *
 * Encapsulates an OpenGL Framebuffer Object (FBO) with a color texture
 * attachment and a depth/stencil renderbuffer attachment.
 */
class Framebuffer {
 public:
  /**
   * @brief Constructs a framebuffer with the specified dimensions.
   * @param width The width of the framebuffer.
   * @param height The height of the framebuffer.
   */
  Framebuffer(int width, int height);

  /** @brief Destroys the framebuffer and its attachments. */
  ~Framebuffer();

  /** @brief Binds the framebuffer as the current render target. */
  void Bind() const;

  /** @brief Unbinds the framebuffer, reverting to the default screen buffer. */
  void Unbind() const;

  /**
   * @brief Resizes the framebuffer.
   * @param width New width.
   * @param height New height.
   */
  void Resize(int width, int height);

  /**
   * @brief Gets the OpenGL ID of the color texture attachment.
   * @return The texture ID.
   */
  [[nodiscard]] unsigned int texture_id() const { return color_attachment_; }

  /**
   * @brief Gets the width of the framebuffer.
   * @return The width.
   */
  [[nodiscard]] int GetWidth() const { return width_; }

  /**
   * @brief Gets the height of the framebuffer.
   * @return The height.
   */
  [[nodiscard]] int GetHeight() const { return height_; }

 private:
  /** @brief Recreates the FBO and its attachments. */
  void Invalidate();

  unsigned int fbo_id_ = 0;
  unsigned int color_attachment_ = 0;
  unsigned int rbo_id_ = 0;  // Depth/Stencil
  int width_;
  int height_;
};

}  // namespace engine::graphics

#endif  // INCLUDE_ENGINE_GRAPHICS_FRAMEBUFFER_H_
