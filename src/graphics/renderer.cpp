#include "graphics/renderer.h"

// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

#include <filesystem>
#include <iostream>

#include "graphics/camera.h"
#include "graphics/text_renderer.h"
#include "graphics/texture.h"
#include "primitive_renderer.h"

namespace engine::graphics {

void Renderer::clear() const {
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::begin_frame(Camera& camera) const {
  // Instruct renders to reset themselves for the frame.
  graphics::PrimitiveRenderer::start_batch(camera.view_projection_matrix());
}

void Renderer::end_frame() const {
  // Flush all renderers.
  graphics::PrimitiveRenderer::finalize_batch();
  graphics::PrimitiveRenderer::render_batch();
}

void Renderer::draw_rect(float x, float y, float width, float height) {
  // Default white color for basic draw_rect call
  glm::vec4 color(1.0f, 1.0f, 1.0f, 1.0f);
  PrimitiveRenderer::submit_quad({x, y}, {width, height}, color);
}

void Renderer::draw_rect(float x, float y, float width, float height, float r,
                         float g, float b) {
  glm::vec4 color(r, g, b, 1.0f);
  PrimitiveRenderer::submit_quad({x, y}, {width, height}, color);
}

void Renderer::draw_quad(const glm::vec2& position, const glm::vec2& size,
                         const glm::vec4& color, float rotation,
                         const glm::vec2& origin) {
  PrimitiveRenderer::submit_quad(position, size, color, rotation, origin);
}

void Renderer::draw_textured_rect(float x, float y, float w, float h,
                                  unsigned int texture_id,
                                  const float tint[4]) {
  glm::vec4 color(1.0f, 1.0f, 1.0f, 1.0f);
  if (tint) {
    color = glm::vec4(tint[0], tint[1], tint[2], tint[3]);
  }

  PrimitiveRenderer::submit_textured_quad({x, y}, {w, h}, texture_id, color);
}

void Renderer::draw_textured_quad(const glm::vec2& position,
                                  const glm::vec2& size,
                                  unsigned int texture_id, float rotation,
                                  const glm::vec4& tint,
                                  const glm::vec2& origin) {
  PrimitiveRenderer::submit_textured_quad(position, size, texture_id, tint,
                                          rotation, origin);
}

void Renderer::draw_textured_quad(const glm::vec2& position,
                                  const glm::vec2& size, const Texture* texture,
                                  float rotation, const glm::vec4& tint,
                                  const glm::vec2& origin) {
  if (texture) {
    PrimitiveRenderer::submit_textured_quad(position, size, texture->id(), tint,
                                            rotation, origin);
  }
}

void Renderer::draw_text(const std::string& font_name, const std::string& text,
                         const glm::vec2& position, float rotation, float scale,
                         const glm::vec4& color) {
  TextRenderer::get().draw_text(font_name, text, position, rotation, scale,
                                color);
}

std::string Renderer::resolve_asset_path(
    const std::string& relative_path) const {
  std::filesystem::path p(relative_path);
  if (p.is_absolute()) return relative_path;
  if (asset_root_path_.empty()) return relative_path;
  return asset_root_path_ + relative_path;
}

// Retrieve the native handle from the window
void Renderer::init(Window& window) {
  // Load Glad
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD\n";
    return;
  }

  // Set default OpenGL state.
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // Note we disable the depth test for 2D games. If we don't then the painter's
  // algorithm doesn't work.
  // As a future improvement we could make this something we automatically
  // enable and disable per frame for different parts of the game.
  glDisable(GL_DEPTH_TEST);

  // Set viewport to window dimensions
  int width, height;
  glfwGetWindowSize(window.native_handle(), &width, &height);
  set_viewport(width, height);

  // Initialize renderers.
  graphics::PrimitiveRenderer::init();
}

void Renderer::shutdown() {
  // Shutdown renderers.
  graphics::PrimitiveRenderer::shutdown();
}

void Renderer::set_viewport(int width, int height) const {
  glViewport(0, 0, width, height);
}

void Renderer::handle_resize(int& width, int& height) const {
  this->set_viewport(width, height);
}

void Renderer::set_asset_root(const std::string& path) {
  if (path.empty()) {
    asset_root_path_ = "";
    return;
  }
  asset_root_path_ = std::filesystem::absolute(path).string();
  // Ensure trailing slash for consistent concatenation
  if (!asset_root_path_.empty() && asset_root_path_.back() != '/' &&
      asset_root_path_.back() != '\\') {
    asset_root_path_ += "/";
  }
}
}  // namespace engine::graphics
