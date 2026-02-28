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

  setup_callbacks();
}

void Window::poll_events() {
  InputManager::get().update_state();
  glfwPollEvents();
  last_frame_time_ = glfwGetTime();
}

double Window::delta_time() const { return glfwGetTime() - last_frame_time_; }

// Private functions

void Window::setup_callbacks() {
  // 1. GLFW Key Callback
  glfwSetKeyCallback(internal_window_, [](GLFWwindow* window, int key,
                                          int scancode, int action, int mods) {
    // Forward the raw event to the InputManager singleton
    InputManager::get().handle_key(key, action);
  });

  // 2. GLFW Mouse Button Callback
  glfwSetMouseButtonCallback(
      internal_window_,
      [](GLFWwindow* window, int button, int action, int mods) {
        InputManager::get().handle_mouse_button(button, action);
      });

  // 3. GLFW Cursor Position Callback
  glfwSetCursorPosCallback(
      internal_window_, [](GLFWwindow* window, double xpos, double ypos) {
        InputManager::get().handle_cursor_position(xpos, ypos);
      });
}

void Window::swap_buffers() const { glfwSwapBuffers(internal_window_); }

}  // namespace engine
