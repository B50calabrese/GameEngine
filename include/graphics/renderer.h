#ifndef GRAPHICS_RENDERER_H
#define GRAPHICS_RENDERER_H

#include "window.h"

namespace engine {
// Forward declaration to give access to private methods.
class Engine;
}  // namespace engine

namespace engine::graphics {
/**
 * The Renderer manages the graphics context and provides the core drawing API.
 * This is a singleton responsible for initializing GLAD and handling global
 * rendering state.
 */
class Renderer {
 public:
  static Renderer& Get() {
    static Renderer instance;
    return instance;
  }

  /**
   * Clears the screen.
   */
  void Clear() const;

  /**
   * Begins the start of the frame for rendering purposes.
   */
  void BeginFrame() const;

  /**
   * Ends the frame and flushes all buffers to be rendered.
   */
  void EndFrame() const;

  void DrawRect(float x, float y, float width, float height);

 private:
  Renderer() = default;
  ~Renderer() = default;

  friend class engine::Engine;

  // Prevent copy/move
  Renderer(const Renderer&) = delete;
  Renderer& operator=(const Renderer&) = delete;

  /**
   * Initializes the OpenGL context using the Window's native handle. This
   * includes calling gladLoadGL().
   * @param window The Window instance that holds the active context.
   */
  void Init(Window& window);

  /**
   * Cleans up any persistent graphics resources.
   */
  void Shutdown();

  /**
   * Sets the OpenGL viewport dimensions.
   */
  void SetViewport(int width, int height) const;

  /**
   * Callback function for window resize events to adjust the viewport.
   */
  void HandleResize(int& width, int& height) const;
};

}  // namespace engine::graphics

#endif  // GRAPHICS_RENDERER_H