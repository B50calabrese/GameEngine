#ifndef GRAPHICS_RENDERER_H
#define GRAPHICS_RENDERER_H

#include "camera.h"
#include "window.h"

namespace engine {
// Forward declaration to give access to private methods.
class Engine;
}  // namespace engine

namespace engine::graphics {
// Manages the graphics context and provides the core drawing API. This is a
// singleton responsible for initializing GLAD and handling global rendering
// state.
class Renderer {
 public:
  // Returns a reference to the singleton `Renderer` instance.
  static Renderer& Get() {
    static Renderer instance;
    return instance;
  }

  // Clears the screen to the default background color.
  void Clear() const;

  // Prepares the renderer for a new frame. This should be called at the
  // beginning of each frame's rendering phase.
  void BeginFrame(Camera& camera) const;

  // Finalizes the frame and presents the rendered image to the screen. This
  // should be called at the end of each frame's rendering phase.
  void EndFrame() const;

  // Draws a rectangle to the screen.
  //
  // @param x The x-coordinate of the top-left corner.
  // @param y The y-coordinate of the top-left corner.
  // @param width The width of the rectangle.
  // @param height The height of the rectangle.
  void DrawRect(float x, float y, float width, float height);

  // Draws a rectangle to the screen using the RGB colors.
  //
  // @param x The x-coordinate of the top-left corner.
  // @param y The y-coordinate of the top-left corner.
  // @param width The width of the rectangle.
  // @param height The height of the rectangle.
  // @param r The r component of the color.
  // @param g The g component of the color.
  // @param b The b component of the color.
  void DrawRect(float x, float y, float width, float height, float r, float g,
                float b);

  // Draws a texturedrectangle to the screen.
  //
  // @param x The x-coordinate of the top-left corner.
  // @param y The y-coordinate of the top-left corner.
  // @param width The width of the rectangle.
  // @param height The height of the rectangle.
  // @param textureID The OpenGL ID of the texture.
  // @param tint Optional RGBA tint (defaults to white).
  void DrawTexturedRect(float x, float y, float w, float h,
                        unsigned int textureID, const float tint[4] = nullptr);

  // Takes a relative path and resolves to the full path.
  std::string ResolveAssetPath(const std::string& relativePath) const;

 private:
  Renderer() = default;
  ~Renderer() = default;

  friend class engine::Engine;

  // Prevent copy/move
  Renderer(const Renderer&) = delete;
  Renderer& operator=(const Renderer&) = delete;

  // Initializes the OpenGL context using the Window's native handle. This
  // includes calling gladLoadGL().
  // @param window The Window instance that holds the active context.
  void Init(Window& window);

  // Cleans up any persistent graphics resources.
  void Shutdown();

  // Sets the OpenGL viewport dimensions.
  void SetViewport(int width, int height) const;

  // Callback function for window resize events to adjust the viewport.
  void HandleResize(int& width, int& height) const;

  // Used to set the asset root path.
  void SetAssetRoot(const std::string& path);

  std::string asset_root_path = "";
};

}  // namespace engine::graphics

#endif  // GRAPHICS_RENDERER_H