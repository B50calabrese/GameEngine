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
 * @brief Manages the application window and its associated OpenGL context.
 *
 * The `Window` class provides a high-level interface for creating and
 * managing a window using GLFW. It handles window creation, event polling,
 * buffer swapping, and maintains information about window dimensions and
 * content scale.
 */
class Window {
 public:
  /**
   * @brief Constructs a new `Window` with the specified dimensions and title.
   *
   * @param width The initial width of the window in pixels.
   * @param height The initial height of the window in pixels.
   * @param name The title of the window displayed in the title bar.
   */
  Window(int width, int height, std::string name);

  /**
   * @brief Returns a pointer to the native GLFW window handle.
   *
   * Use this when interfacing directly with GLFW or other libraries that
   * require the raw GLFW window pointer.
   *
   * @return Pointer to the underlying GLFWwindow.
   */
  GLFWwindow* native_handle() const { return internal_window_; }

  /**
   * @brief Checks if the window is currently running and should remain open.
   * @return True if the window is open and not requested to close.
   */
  bool IsRunning() const;

  /**
   * @brief Checks if the window has been requested to close.
   * @return True if the window should close.
   */
  bool ShouldClose() const;

  /**
   * @brief Swaps the front and back buffers to display the rendered frame.
   */
  void SwapBuffers() const;

  /**
   * @brief Processes all pending events (input, window resize, etc.).
   *
   * This should be called once per frame in the main loop.
   */
  void PollEvents();

  /**
   * @brief Returns the time elapsed since the last frame was processed.
   * @return The delta time in seconds.
   */
  double delta_time() const;

  /**
   * @brief Gets the current width of the window's framebuffer.
   * @return Width in pixels.
   */
  int width() const { return width_; }

  /**
   * @brief Gets the current height of the window's framebuffer.
   * @return Height in pixels.
   */
  int height() const { return height_; }

  /**
   * @brief Gets the horizontal content scale factor.
   *
   * This is useful for High-DPI displays where screen coordinates may differ
   * from pixel coordinates.
   *
   * @return The horizontal scale factor.
   */
  float content_scale_x() const { return content_scale_x_; }

  /**
   * @brief Gets the vertical content scale factor.
   *
   * This is useful for High-DPI displays where screen coordinates may differ
   * from pixel coordinates.
   *
   * @return The vertical scale factor.
   */
  float content_scale_y() const { return content_scale_y_; }

 private:
  friend class Application;

  GLFWwindow* internal_window_;
  int width_;
  int height_;
  float content_scale_x_ = 1.0f;
  float content_scale_y_ = 1.0f;

  double last_frame_time_;

  /**
   * @brief Sets up GLFW callbacks for input events.
   */
  void SetupCallbacks();

  // Prevent copy/move to enforce a single instance handled by Engine
  Window(const Window&) = delete;
  Window& operator=(const Window&) = delete;
};
}  // namespace engine

#endif  // INCLUDE_ENGINE_CORE_WINDOW_H_
