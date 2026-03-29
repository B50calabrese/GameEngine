/**
 * @file window.cpp
 * @brief Window class implementation.
 */

#include <engine/core/window.h>

// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

#include <string>

#include <engine/graphics/renderer.h>
#include <engine/input/input_manager.h>

#include <imgui.h>

namespace engine {

Window::Window(int width, int height, std::string name)
    : width_(width), height_(height) {
  internal_window_ =
      glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);

  glfwMakeContextCurrent(internal_window_);
  glfwSwapInterval(1);  // Enable V-Sync (swap interval 1)

  last_frame_time_ = glfwGetTime();

  SetupCallbacks();
}

void Window::PollEvents() {
  InputManager::Get().UpdateState();
  glfwPollEvents();
  last_frame_time_ = glfwGetTime();
}

double Window::delta_time() const { return glfwGetTime() - last_frame_time_; }

bool Window::IsRunning() const {
  return !glfwWindowShouldClose(internal_window_);
}

bool Window::ShouldClose() const {
  return glfwWindowShouldClose(internal_window_);
}

void Window::SwapBuffers() const { glfwSwapBuffers(internal_window_); }

// Private functions

void Window::SetupCallbacks() {
  glfwSetWindowUserPointer(internal_window_, this);

  // 0. GLFW Framebuffer Size Callback
  glfwSetFramebufferSizeCallback(
      internal_window_, [](GLFWwindow* window, int width, int height) {
        Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
        if (win) {
          win->width_ = width;
          win->height_ = height;
          graphics::Renderer::Get().HandleResize(width, height);
          InputManager::Get().HandleResize(width, height);
        }
      });

  // 1. GLFW Key Callback
  glfwSetKeyCallback(internal_window_, [](GLFWwindow* window, int key,
                                          int scancode, int action, int mods) {
    if (ImGui::GetCurrentContext() != nullptr &&
        ImGui::GetIO().WantCaptureKeyboard) {
      return;
    }
    // Forward the raw event to the InputManager singleton
    InputManager::Get().HandleKey(key, action);
  });

  // 2. GLFW Mouse Button Callback
  glfwSetMouseButtonCallback(
      internal_window_,
      [](GLFWwindow* window, int button, int action, int mods) {
        if (ImGui::GetCurrentContext() != nullptr &&
            ImGui::GetIO().WantCaptureMouse) {
          return;
        }
        InputManager::Get().HandleMouseButton(button, action);
      });

  // 3. GLFW Cursor Position Callback
  glfwSetCursorPosCallback(
      internal_window_, [](GLFWwindow* window, double xpos, double ypos) {
        InputManager::Get().HandleCursorPosition(xpos, ypos);
      });

  // 4. GLFW Char Callback
  glfwSetCharCallback(internal_window_,
                      [](GLFWwindow* window, unsigned int codepoint) {
                        if (ImGui::GetCurrentContext() != nullptr &&
                            ImGui::GetIO().WantCaptureKeyboard) {
                          return;
                        }
                        InputManager::Get().HandleChar(codepoint);
                      });
}

}  // namespace engine
