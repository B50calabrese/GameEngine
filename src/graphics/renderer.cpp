#include "graphics/renderer.h"

// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

#include <filesystem>
#include <iostream>

#include "graphics/camera.h"
#include "primitive_renderer.h"

namespace engine::graphics {

void Renderer::Clear() const {
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::BeginFrame(Camera& camera) const {
  // Instruct renders to reset themselves for the frame.
  graphics::PrimitiveRenderer::StartBatch(camera.view_projection_matrix());
}

void Renderer::EndFrame() const {
  // Flush all renderers.
  graphics::PrimitiveRenderer::FinalizeBatch();
  graphics::PrimitiveRenderer::RenderBatch();
}

void Renderer::DrawRect(float x, float y, float width, float height) {
  // Default white color for basic DrawRect call
  float color[4] = {1.0f, 1.0f, 1.0f, 1.0f};
  PrimitiveRenderer::SubmitQuad(x, y, width, height, color);
}

void Renderer::DrawRect(float x, float y, float width, float height, float r,
                        float g, float b) {
  float color[4] = {r, g, b, 1.0f};
  PrimitiveRenderer::SubmitQuad(x, y, width, height, color);
}

void Renderer::DrawTexturedRect(float x, float y, float w, float h,
                                unsigned int texture_id, const float tint[4]) {
  static float default_tint[4] = {1.0f, 1.0f, 1.0f, 1.0f};
  const float* color = tint ? tint : default_tint;

  PrimitiveRenderer::SubmitTexturedQuad(x, y, w, h, texture_id, color);
}

std::string Renderer::ResolveAssetPath(const std::string& relative_path) const {
  std::filesystem::path p(relative_path);
  if (p.is_absolute()) return relative_path;
  return asset_root_path_ + relative_path;
}

// Retrieve the native handle from the window
void Renderer::Init(Window& window) {
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
  SetViewport(width, height);

  // Initialize renderers.
  graphics::PrimitiveRenderer::Init();
}

void Renderer::Shutdown() {
  // Shutdown renderers.
  graphics::PrimitiveRenderer::Shutdown();
}

void Renderer::SetViewport(int width, int height) const {
  glViewport(0, 0, width, height);
}

void Renderer::HandleResize(int& width, int& height) const {
  this->SetViewport(width, height);
}

void Renderer::SetAssetRoot(const std::string& path) {
  asset_root_path_ = std::filesystem::absolute(path).string();
  // Ensure trailing slash for consistent concatenation
  if (!asset_root_path_.empty() && asset_root_path_.back() != '/' &&
      asset_root_path_.back() != '\\') {
    asset_root_path_ += "/";
  }
}
}  // namespace engine::graphics
