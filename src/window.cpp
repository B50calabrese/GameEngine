#include "window.h"

#include <GLFW/glfw3.h>

#include <string>

#include "input_manager.h"

namespace engine {

Window::Window(int width, int height, std::string name) {
  this->internal_window =
      glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);

  glfwMakeContextCurrent(this->internal_window);
  glfwSwapInterval(1);  // Enable V-Sync (swap interval 1)

  this->last_frame_time = glfwGetTime();

  this->SetupCallbacks();
}

void Window::PollEvents() {
  InputManager::Get().UpdateState();
  glfwPollEvents();
  this->last_frame_time = glfwGetTime();
}

double Window::GetDeltaTime() const {
  return glfwGetTime() - this->last_frame_time;
}

// Private functions

void Window::SetupCallbacks() {
  // 1. GLFW Key Callback
  glfwSetKeyCallback(
      this->internal_window,
      [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        // Forward the raw event to the InputManager singleton
        InputManager::Get().HandleKey(key, action);
      });

  // 2. GLFW Mouse Button Callback
  glfwSetMouseButtonCallback(
      this->internal_window,
      [](GLFWwindow* window, int button, int action, int mods) {
        InputManager::Get().HandleMouseButton(button, action);
      });

  // 3. GLFW Cursor Position Callback
  glfwSetCursorPosCallback(
      this->internal_window, [](GLFWwindow* window, double xpos, double ypos) {
        InputManager::Get().HandleCursorPosition(xpos, ypos);
      });
}

void Window::SwapBuffers() const { glfwSwapBuffers(this->internal_window); }

}  // namespace engine