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
 *
 * This class is a wrapper around the underlying GLFW windowing library.
 */
class Window {
 public:
  /**
   * @brief Constructs a new `Window` with the specified dimensions and title.
   *
   * @param width The width of the window in pixels.
   * @param height The height of the window in pixels.
   * @param name The title of the window.
   */
  Window(int width, int height, std::string name);

  /**
   * @brief Returns a pointer to the native GLFW window handle.
   * @return Pointer to the GLFWwindow.
   */
  GLFWwindow* native_handle() const { return internal_window_; }

  /**
   * @brief Checks if the window should remain open.
   * @return `true` if the window should stay open, `false` otherwise.
   */
  bool IsRunning() const;

  /**
   * @brief Checks if the window should close.
   * @return `true` if the window should close, `false` otherwise.
   */
  bool ShouldClose() const;

  /**
   * @brief Swaps the front and back buffers.
   */
  void SwapBuffers() const;

  /**
   * @brief Processes all pending events, such as keyboard and mouse input.
   */
  void PollEvents();

  /**
   * @brief Calculates and returns the time elapsed since the last frame.
   * @return The delta time in seconds.
   */
  double delta_time() const;

  /**
   * @brief Gets the width of the window.
   * @return Width in pixels.
   */
  int width() const { return width_; }

  /**
   * @brief Gets the height of the window.
   * @return Height in pixels.
   */
  int height() const { return height_; }

  /**
   * @brief Gets the horizontal content scale.
   * @return The X content scale.
   */
  float content_scale_x() const { return content_scale_x_; }

  /**
   * @brief Gets the vertical content scale.
   * @return The Y content scale.
   */
  float content_scale_y() const { return content_scale_y_; }

 private:
  friend class Application;

  GLFWwindow* internal_window_;
  int width_;   // Framebuffer width
  int height_;  // Framebuffer height
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
