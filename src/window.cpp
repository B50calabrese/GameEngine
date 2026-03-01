#include "window.h"

// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

#include <string>

#include "input_manager.h"

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

// Private functions

void Window::SetupCallbacks() {
  // 1. GLFW Key Callback
  glfwSetKeyCallback(internal_window_, [](GLFWwindow* window, int key,
                                          int scancode, int action, int mods) {
    // Forward the raw event to the InputManager singleton
    InputManager::Get().HandleKey(key, action);
  });

  // 2. GLFW Mouse Button Callback
  glfwSetMouseButtonCallback(
      internal_window_,
      [](GLFWwindow* window, int button, int action, int mods) {
        InputManager::Get().HandleMouseButton(button, action);
      });

  // 3. GLFW Cursor Position Callback
  glfwSetCursorPosCallback(
      internal_window_, [](GLFWwindow* window, double xpos, double ypos) {
        InputManager::Get().HandleCursorPosition(xpos, ypos);
      });
}

void Window::SwapBuffers() const { glfwSwapBuffers(internal_window_); }

}  // namespace engine
