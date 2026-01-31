#ifndef INCLUDE_GRAPHICS_RENDERER_H_
#define INCLUDE_GRAPHICS_RENDERER_H_

#include <string>

#include "camera.h"
#include "window.h"

namespace engine {
// Forward declaration to give access to private methods.
class Engine;
}  // namespace engine

namespace engine::graphics {
/**
 * @brief Manages the graphics context and provides the core drawing API.
 *
 * This is a singleton responsible for initializing GLAD and handling global
 * rendering state.
 */
class Renderer {
 public:
  /**
   * @brief Returns a reference to the singleton `Renderer` instance.
   * @return Reference to the Renderer.
   */
  static Renderer& Get() {
    static Renderer instance;
    return instance;
  }

  /** @brief Clears the screen to the default background color. */
  void Clear() const;

  /**
   * @brief Prepares the renderer for a new frame.
   *
   * This should be called at the beginning of each frame's rendering phase.
   *
   * @param camera The camera to use for this frame.
   */
  void BeginFrame(Camera& camera) const;

  /**
   * @brief Finalizes the frame and presents the rendered image to the screen.
   *
   * This should be called at the end of each frame's rendering phase.
   */
  void EndFrame() const;

  /**
   * @brief Draws a rectangle to the screen.
   *
   * @param x The x-coordinate of the top-left corner.
   * @param y The y-coordinate of the top-left corner.
   * @param width The width of the rectangle.
   * @param height The height of the rectangle.
   */
  void DrawRect(float x, float y, float width, float height);

  /**
   * @brief Draws a rectangle to the screen using the RGB colors.
   *
   * @param x The x-coordinate of the top-left corner.
   * @param y The y-coordinate of the top-left corner.
   * @param width The width of the rectangle.
   * @param height The height of the rectangle.
   * @param r The r component of the color.
   * @param g The g component of the color.
   * @param b The b component of the color.
   */
  void DrawRect(float x, float y, float width, float height, float r, float g,
                float b);

  /**
   * @brief Draws a textured rectangle to the screen.
   *
   * @param x The x-coordinate of the top-left corner.
   * @param y The y-coordinate of the top-left corner.
   * @param w The width of the rectangle.
   * @param h The height of the rectangle.
   * @param textureID The OpenGL ID of the texture.
   * @param tint Optional RGBA tint (defaults to white).
   */
  void DrawTexturedRect(float x, float y, float w, float h,
                        unsigned int texture_id, const float tint[4] = nullptr);

  /**
   * @brief Takes a relative path and resolves to the full path.
   * @param relative_path The relative path to resolve.
   * @return The absolute path as a string.
   */
  std::string ResolveAssetPath(const std::string& relative_path) const;

 private:
  Renderer() = default;
  ~Renderer() = default;

  friend class engine::Engine;

  // Prevent copy/move
  Renderer(const Renderer&) = delete;
  Renderer& operator=(const Renderer&) = delete;

  /**
   * @brief Initializes the OpenGL context using the Window's native handle.
   *
   * This includes calling gladLoadGL().
   *
   * @param window The Window instance that holds the active context.
   */
  void Init(Window& window);

  /** @brief Cleans up any persistent graphics resources. */
  void Shutdown();

  /**
   * @brief Sets the OpenGL viewport dimensions.
   * @param width Viewport width.
   * @param height Viewport height.
   */
  void SetViewport(int width, int height) const;

  /**
   * @brief Callback function for window resize events to adjust the viewport.
   * @param width New width.
   * @param height New height.
   */
  void HandleResize(int& width, int& height) const;

  /**
   * @brief Used to set the asset root path.
   * @param path The root path.
   */
  void SetAssetRoot(const std::string& path);

  std::string asset_root_path_ = "";
};

}  // namespace engine::graphics

#endif  // INCLUDE_GRAPHICS_RENDERER_H_
