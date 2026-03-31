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

Window::Window(int width, int height, std::string name)
    : width_(width), height_(height) {
  native_window_ =
      glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);

  glfwMakeContextCurrent(native_window_);
  glfwSwapInterval(1);

  last_frame_time_ = glfwGetTime();

  SetupCallbacks();
}

void Window::PollEvents() {
  InputManager::Get().UpdateState();
  glfwPollEvents();
  last_frame_time_ = glfwGetTime();
}

double Window::GetDeltaTime() const { return glfwGetTime() - last_frame_time_; }

bool Window::IsRunning() const {
  return !glfwWindowShouldClose(native_window_);
}

bool Window::ShouldClose() const {
  return glfwWindowShouldClose(native_window_);
}

void Window::SwapBuffers() const { glfwSwapBuffers(native_window_); }

void Window::SetupCallbacks() {
  glfwSetWindowUserPointer(native_window_, this);

  glfwSetFramebufferSizeCallback(
      native_window_, [](GLFWwindow* window, int width, int height) {
        Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
        if (win) {
          win->width_ = width;
          win->height_ = height;
          graphics::Renderer::Get().HandleResize(width, height);
          InputManager::Get().HandleResize(width, height);
        }
      });

  glfwSetKeyCallback(native_window_, [](GLFWwindow* window, int key,
                                        int scancode, int action, int mods) {
    if (ImGui::GetCurrentContext() != nullptr &&
        ImGui::GetIO().WantCaptureKeyboard) {
      return;
    }
    InputManager::Get().HandleKey(key, action);
  });

  glfwSetMouseButtonCallback(
      native_window_, [](GLFWwindow* window, int button, int action, int mods) {
        if (ImGui::GetCurrentContext() != nullptr &&
            ImGui::GetIO().WantCaptureMouse) {
          return;
        }
        InputManager::Get().HandleMouseButton(button, action);
      });

  glfwSetCursorPosCallback(
      native_window_, [](GLFWwindow* window, double xpos, double ypos) {
        InputManager::Get().HandleCursorPosition(xpos, ypos);
      });

  glfwSetCharCallback(native_window_,
                      [](GLFWwindow* window, unsigned int codepoint) {
                        if (ImGui::GetCurrentContext() != nullptr &&
                            ImGui::GetIO().WantCaptureKeyboard) {
                          return;
                        }
                        InputManager::Get().HandleChar(codepoint);
                      });
}

}  // namespace engine
