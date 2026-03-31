/**
 * @file window.h
 * @brief Manages the application window and OpenGL context.
 */

#ifndef INCLUDE_ENGINE_CORE_WINDOW_H_
#define INCLUDE_ENGINE_CORE_WINDOW_H_

#include <string>

struct GLFWwindow;

namespace engine {

class Application;

/**
 * @brief Manages the application window and its associated context.
 */
class Window {
 public:
  /** @brief Constructs a new `Window`. */
  Window(int width, int height, std::string name);

  /** @brief Gets the native GLFW window handle. */
  GLFWwindow* GetNativeHandle() const { return native_window_; }

  /** @brief Returns true if the window is running. */
  bool IsRunning() const;

  /** @brief Returns true if the window should close. */
  bool ShouldClose() const;

  /** @brief Swaps buffers. */
  void SwapBuffers() const;

  /** @brief Processes events. */
  void PollEvents();

  /** @brief Gets delta time. */
  double GetDeltaTime() const;

  /** @brief Gets window width. */
  int GetWidth() const { return width_; }

  /** @brief Gets window height. */
  int GetHeight() const { return height_; }

 private:
  friend class Application;

  GLFWwindow* native_window_;
  int width_;
  int height_;
  double last_frame_time_;

  /** @brief Sets up GLFW callbacks. */
  void SetupCallbacks();

  Window(const Window&) = delete;
  Window& operator=(const Window&) = delete;
};

}  // namespace engine

#endif  // INCLUDE_ENGINE_CORE_WINDOW_H_
