#ifndef WINDOW_H
#define WINDOW_H

#include <string>

// Forward declaration
struct GLFWwindow;

namespace engine {
class Window {
 public:
  Window(int width, int height, std::string name);

  GLFWwindow* GetNativeHandle() const { return this->internal_window; }

  // TODO: Implement
  bool IsRunning() const { return true; }

  void PollEvents();

  double GetDeltaTime() const;

 private:
  GLFWwindow* internal_window;

  double last_frame_time;

  void SetupCallbacks();

  // Prevent copy/move to enforce a single instance handled by Engine
  Window(const Window&) = delete;
  Window& operator=(const Window&) = delete;
};
}  // namespace engine

#endif  // WINDOW_H