#include "graphics/renderer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "primitive_renderer.h"

namespace engine::graphics {

void Renderer::Clear() const {
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::BeginFrame() const {
  // Instruct renders to reset themselves for the frame.
  graphics::PrimitiveRenderer::StartBatch();
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
  glEnable(GL_DEPTH_TEST);

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

}  // namespace engine::graphics