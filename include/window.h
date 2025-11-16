#pragma once

#include <string>

// Forward declaration
struct GLFWwindow;

namespace engine {

// The Window class is responsible for creating and managing the main
// application window.
class Window {
 public:
  // @brief Creates a new window.
  // @param width The width of the window.
  // @param height The height of the window.
  // @param name The title of the window.
  Window(int width, int height, const std::string& name);

  // @brief Returns the native handle to the window.
  // @return The native handle to the window.
  GLFWwindow* GetNativeHandle() const { return window_; }

  // @brief Polls for events.
  void PollEvents();

 private:
  GLFWwindow* window_ = nullptr;

  void SetupCallbacks();

  // Prevent copy/move to enforce a single instance handled by Engine
  Window(const Window&) = delete;
  Window& operator=(const Window&) = delete;
};

}  // namespace engine
