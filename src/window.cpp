#include "window.h"

#include <GLFW/glfw3.h>

#include <string>

#include "input_manager.h"

namespace engine {

Window::Window(const int width, const int height, const std::string& name) {
  window_ = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);

  glfwMakeContextCurrent(window_);
  glfwSwapInterval(1);  // Enable V-Sync (swap interval 1)

  SetupCallbacks();
}

void Window::PollEvents() {
  InputManager::Get().UpdateState();
  glfwPollEvents();
}

// Private functions

void Window::SetupCallbacks() {
  // 1. GLFW Key Callback
  glfwSetKeyCallback(
      window_,
      [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        // Forward the raw event to the InputManager singleton
        InputManager::Get().HandleKey(key, action);
      });

  // 2. GLFW Mouse Button Callback
  glfwSetMouseButtonCallback(
      window_, [](GLFWwindow* window, int button, int action, int mods) {
        InputManager::Get().HandleMouseButton(button, action);
      });

  // 3. GLFW Cursor Position Callback
  glfwSetCursorPosCallback(window_,
                           [](GLFWwindow* window, double xpos, double ypos) {
                             InputManager::Get().HandleCursorPosition(xpos, ypos);
                           });
}

}  // namespace engine
