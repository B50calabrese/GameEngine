/**
 * @file window.cpp
 * @brief Window class implementation.
 */

#include <engine/core/window.h>

// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

#include <imgui.h>

#include <string>

#include <engine/graphics/renderer.h>
#include <engine/input/input_manager.h>

namespace engine {

Window::Window(int width, int height, std::string name) {
  internal_window_ =
      glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);

  glfwMakeContextCurrent(internal_window_);
  glfwSwapInterval(1);  // Enable V-Sync (swap interval 1)

  // Use framebuffer size for actual pixel dimensions
  int fb_width, fb_height;
  glfwGetFramebufferSize(internal_window_, &fb_width, &fb_height);
  width_ = fb_width;
  height_ = fb_height;

  // Calculate content scale (framebuffer size / window size)
  int win_width, win_height;
  glfwGetWindowSize(internal_window_, &win_width, &win_height);
  content_scale_x_ = static_cast<float>(fb_width) / static_cast<float>(win_width);
  content_scale_y_ = static_cast<float>(fb_height) / static_cast<float>(win_height);

  // Ensure InputManager knows the initial window size for mouse Y calculations
  InputManager::Get().HandleResize(width_, height_, content_scale_x_,
                                   content_scale_y_);

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

          // Re-calculate content scale
          int win_width, win_height;
          glfwGetWindowSize(window, &win_width, &win_height);
          win->content_scale_x_ =
              static_cast<float>(width) / static_cast<float>(win_width);
          win->content_scale_y_ =
              static_cast<float>(height) / static_cast<float>(win_height);

          graphics::Renderer::Get().HandleResize(width, height);
          InputManager::Get().HandleResize(width, height, win->content_scale_x_,
                                           win->content_scale_y_);
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
