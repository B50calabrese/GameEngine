/**
 * @file framebuffer.cpp
 * @brief Framebuffer class implementation.
 */

/**
 * @dir src/engine/graphics
 * @brief Rendering system internals.
 */

#include <engine/graphics/framebuffer.h>
#include <engine/util/logger.h>

namespace engine::graphics {

Framebuffer::Framebuffer(int width, int height)
    : width_(width), height_(height) {
  Invalidate();
}

Framebuffer::~Framebuffer() {
  glDeleteFramebuffers(1, &fbo_id_);
  glDeleteTextures(1, &color_attachment_);
  glDeleteRenderbuffers(1, &rbo_id_);
}

void Framebuffer::Bind() const {
  glBindFramebuffer(GL_FRAMEBUFFER, fbo_id_);
  glViewport(0, 0, width_, height_);
}

void Framebuffer::Unbind() const { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

void Framebuffer::Resize(int width, int height) {
  if (width == width_ && height == height_) {
    return;
  }
  width_ = width;
  height_ = height;
  Invalidate();
}

void Framebuffer::Invalidate() {
  if (fbo_id_) {
    glDeleteFramebuffers(1, &fbo_id_);
    glDeleteTextures(1, &color_attachment_);
    glDeleteRenderbuffers(1, &rbo_id_);
  }

  glGenFramebuffers(1, &fbo_id_);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo_id_);

  // Color attachment
  glGenTextures(1, &color_attachment_);
  glBindTexture(GL_TEXTURE_2D, color_attachment_);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width_, height_, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, nullptr);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         color_attachment_, 0);

  // Depth and Stencil attachment
  glGenRenderbuffers(1, &rbo_id_);
  glBindRenderbuffer(GL_RENDERBUFFER, rbo_id_);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width_, height_);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                            GL_RENDERBUFFER, rbo_id_);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    LOG_ERR("Framebuffer is incomplete!");
  }

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

}  // namespace engine::graphics
