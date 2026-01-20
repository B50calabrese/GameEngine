#include "graphics/renderer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

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
  graphics::PrimitiveRenderer::StartBatch(camera.GetViewProjectionMatrix());
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
                                unsigned int textureID, const float tint[4]) {
  static float defaultTint[4] = {1.0f, 1.0f, 1.0f, 1.0f};
  const float* color = tint ? tint : defaultTint;

  PrimitiveRenderer::SubmitTexturedQuad(x, y, w, h, textureID, color);
}

std::string Renderer::ResolveAssetPath(const std::string& relativePath) const {
  std::filesystem::path p(relativePath);
  if (p.is_absolute()) return relativePath;
  return asset_root_path + relativePath;
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
  glfwGetWindowSize(window.GetNativeHandle(), &width, &height);
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
  asset_root_path = std::filesystem::absolute(path).string();
  // Ensure trailing slash for consistent concatenation
  if (!asset_root_path.empty() && asset_root_path.back() != '/' &&
      asset_root_path.back() != '\\') {
    asset_root_path += "/";
  }
}
}  // namespace engine::graphics