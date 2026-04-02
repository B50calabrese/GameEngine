/**
 * @file window.h
 * @brief Manages the application window and OpenGL context.
 */

#ifndef INCLUDE_ENGINE_CORE_WINDOW_H_
#define INCLUDE_ENGINE_CORE_WINDOW_H_

#include <string>

// Forward declaration
struct GLFWwindow;

namespace engine {
// Forward declaration
class Application;

/**
 * @brief Manages the application window and its associated context.
 */
class Window {
 public:
  /**
   * @brief Constructs a new `Window`.
   * @param width The width.
   * @param height The height.
   * @param name The title.
   */
  Window(int width, int height, std::string name);

  /**
   * @brief Gets the native GLFW window handle.
   * @return Pointer to the GLFWwindow.
   */
  GLFWwindow* GetNativeHandle() const { return internal_window_; }

  /**
   * @brief Checks if the window is running.
   * @return `true` if it's running.
   */
  bool IsRunning() const;

  /**
   * @brief Checks if the window should close.
   * @return `true` if it should close.
   */
  bool ShouldClose() const;

  /**
   * @brief Swaps buffers.
   */
  void SwapBuffers() const;

  /**
   * @brief Processes all pending events.
   */
  void PollEvents();

  /**
   * @brief Gets the time elapsed since the last frame.
   * @return The delta time in seconds.
   */
  double GetDeltaTime() const;

  /**
   * @brief Gets the width of the window.
   * @return Width in pixels.
   */
  int GetWidth() const { return width_; }

  /**
   * @brief Gets the height of the window.
   * @return Height in pixels.
   */
  int GetHeight() const { return height_; }

 private:
  friend class Application;

  GLFWwindow* internal_window_;
  int width_;
  int height_;

  double last_frame_time_;

  /**
   * @brief Sets up GLFW callbacks.
   */
  void SetupCallbacks();

  Window(const Window&) = delete;
  Window& operator=(const Window&) = delete;
};
}  // namespace engine

#endif  // INCLUDE_ENGINE_CORE_WINDOW_H_
