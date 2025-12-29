#include "graphics/renderer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

namespace engine::graphics {

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
}

void Renderer::Shutdown() {
  // Perform any necessary cleanup for the renderer
}

void Renderer::SetViewport(int width, int height) const {
  glViewport(0, 0, width, height);
}

void Renderer::HandleResize(int& width, int& height) const {
  this->SetViewport(width, height);
}

void Renderer::Clear() const {
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

}  // namespace engine::graphics