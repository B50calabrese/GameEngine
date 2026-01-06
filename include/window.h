#ifndef WINDOW_H
#define WINDOW_H

#include <string>

// Forward declaration
struct GLFWwindow;

namespace engine {
// Forward declaration
class Application;

// Manages the application window and its associated context. This class is a
// wrapper around the underlying GLFW windowing library.
class Window {
 public:
  // Constructs a new `Window` with the specified dimensions and title.
  //
  // @param width The width of the window in pixels.
  // @param height The height of the window in pixels.
  // @param name The title of the window.
  Window(int width, int height, std::string name);

  // Returns a pointer to the native GLFW window handle.
  GLFWwindow* GetNativeHandle() const { return this->internal_window; }

  // TODO: Implement
  // Checks if the window should remain open.
  //
  // @return `true` if the window should stay open, `false` otherwise.
  bool IsRunning() const { return true; }

  // Processes all pending events, such as keyboard and mouse input.
  void PollEvents();

  // Calculates and returns the time elapsed since the last frame.
  //
  // @return The delta time in seconds.
  double GetDeltaTime() const;

 private:
  friend class Application;

  GLFWwindow* internal_window;

  double last_frame_time;

  void SetupCallbacks();

  void SwapBuffers() const;

  // Prevent copy/move to enforce a single instance handled by Engine
  Window(const Window&) = delete;
  Window& operator=(const Window&) = delete;
};
}  // namespace engine

#endif  // WINDOW_H