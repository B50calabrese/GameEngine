#ifndef INCLUDE_WINDOW_H_
#define INCLUDE_WINDOW_H_

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
  bool is_running() const { return true; }

  /**
   * @brief Processes all pending events, such as keyboard and mouse input.
   */
  void poll_events();

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

 private:
  friend class Application;

  GLFWwindow* internal_window_;
  int width_;
  int height_;

  double last_frame_time_;

  /**
   * @brief Sets up GLFW callbacks for input events.
   */
  void setup_callbacks();

  /**
   * @brief Swaps the front and back buffers.
   */
  void swap_buffers() const;

  // Prevent copy/move to enforce a single instance handled by Engine
  Window(const Window&) = delete;
  Window& operator=(const Window&) = delete;
};
}  // namespace engine

#endif  // INCLUDE_WINDOW_H_
