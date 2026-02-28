#ifndef INCLUDE_GRAPHICS_RENDERER_H_
#define INCLUDE_GRAPHICS_RENDERER_H_

#include <glm/glm.hpp>
#include <string>

#include "camera.h"
#include "window.h"

namespace engine {
// Forward declaration to give access to private methods.
class Engine;
}  // namespace engine

namespace engine::graphics {

class Texture;

/**
 * @brief Manages the graphics context and provides the core drawing API.
 *
 * This is a singleton responsible for initializing GLAD and handling global
 * rendering state.
 *
 * The coordinate system by default has (0,0) at the bottom-left of the window,
 * matching the default orthographic projection of the Camera.
 */
class Renderer {
 public:
  /**
   * @brief Returns a reference to the singleton `Renderer` instance.
   * @return Reference to the Renderer.
   */
  static Renderer& get() {
    static Renderer instance;
    return instance;
  }

  /** @brief Clears the screen to the default background color. */
  void clear() const;

  /**
   * @brief Prepares the renderer for a new frame.
   *
   * This should be called at the beginning of each frame's rendering phase.
   *
   * @param camera The camera to use for this frame.
   */
  void begin_frame(Camera& camera) const;

  /**
   * @brief Finalizes the frame and presents the rendered image to the screen.
   *
   * This should be called at the end of each frame's rendering phase.
   */
  void end_frame() const;

  /**
   * @brief Draws a solid white rectangle to the screen.
   *
   * @param x The x-coordinate of the bottom-left corner.
   * @param y The y-coordinate of the bottom-left corner.
   * @param width The width of the rectangle.
   * @param height The height of the rectangle.
   */
  void draw_rect(float x, float y, float width, float height);

  /**
   * @brief Draws a solid colored rectangle to the screen using RGB values.
   *
   * @param x The x-coordinate of the bottom-left corner.
   * @param y The y-coordinate of the bottom-left corner.
   * @param width The width of the rectangle.
   * @param height The height of the rectangle.
   * @param r The red component (0-1).
   * @param g The green component (0-1).
   * @param b The blue component (0-1).
   */
  void draw_rect(float x, float y, float width, float height, float r, float g,
                 float b);

  /**
   * @brief Draws a colored quad with full transformation support.
   *
   * @param position The world-space position.
   * @param size The width and height of the quad.
   * @param color The RGBA color.
   * @param rotation The rotation in degrees (counter-clockwise).
   * @param origin The origin point for rotation and positioning relative to
   * size (e.g., (0,0) for bottom-left, (0.5, 0.5) for center).
   */
  void draw_quad(const glm::vec2& position, const glm::vec2& size,
                 const glm::vec4& color, float rotation = 0.0f,
                 const glm::vec2& origin = {0.0f, 0.0f});

  /**
   * @brief Draws a textured rectangle to the screen.
   *
   * @param x The x-coordinate of the bottom-left corner.
   * @param y The y-coordinate of the bottom-left corner.
   * @param w The width of the rectangle.
   * @param h The height of the rectangle.
   * @param texture_id The OpenGL ID of the texture.
   * @param tint Optional RGBA tint (defaults to white).
   */
  void draw_textured_rect(float x, float y, float w, float h,
                          unsigned int texture_id,
                          const float tint[4] = nullptr);

  /**
   * @brief Draws a textured quad with full transformation support.
   *
   * @param position The world-space position.
   * @param size The width and height of the quad.
   * @param texture_id The OpenGL ID of the texture.
   * @param rotation The rotation in degrees (counter-clockwise).
   * @param tint The RGBA tint color.
   * @param origin The origin point for rotation and positioning relative to
   * size (e.g., (0,0) for bottom-left, (0.5, 0.5) for center).
   */
  void draw_textured_quad(const glm::vec2& position, const glm::vec2& size,
                          unsigned int texture_id, float rotation = 0.0f,
                          const glm::vec4& tint = glm::vec4(1.0f),
                          const glm::vec2& origin = {0.0f, 0.0f});

  /**
   * @brief Draws a textured quad with full transformation support.
   *
   * @param position The world-space position.
   * @param size The width and height of the quad.
   * @param texture The Texture resource object.
   * @param rotation The rotation in degrees (counter-clockwise).
   * @param tint The RGBA tint color.
   * @param origin The origin point for rotation and positioning relative to
   * size (e.g., (0,0) for bottom-left, (0.5, 0.5) for center).
   */
  void draw_textured_quad(const glm::vec2& position, const glm::vec2& size,
                          const Texture* texture, float rotation = 0.0f,
                          const glm::vec4& tint = glm::vec4(1.0f),
                          const glm::vec2& origin = {0.0f, 0.0f});

  /**
   * @brief Renders text with full transformation support.
   *
   * @param font_name The name of the loaded font.
   * @param text The string to render.
   * @param position The baseline origin of the text.
   * @param rotation The rotation in degrees (counter-clockwise).
   * @param scale The scale factor.
   * @param color The RGBA color.
   */
  void draw_text(const std::string& font_name, const std::string& text,
                 const glm::vec2& position, float rotation = 0.0f,
                 float scale = 1.0f, const glm::vec4& color = glm::vec4(1.0f));

  /**
   * @brief Takes a relative path and resolves to the full path.
   * @param relative_path The relative path to resolve.
   * @return The absolute path as a string.
   */
  std::string resolve_asset_path(const std::string& relative_path) const;

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
  void init(Window& window);

  /** @brief Cleans up any persistent graphics resources. */
  void shutdown();

  /**
   * @brief Sets the OpenGL viewport dimensions.
   * @param width Viewport width.
   * @param height Viewport height.
   */
  void set_viewport(int width, int height) const;

  /**
   * @brief Callback function for window resize events to adjust the viewport.
   * @param width New width.
   * @param height New height.
   */
  void handle_resize(int& width, int& height) const;

  /**
   * @brief Used to set the asset root path.
   * @param path The root path.
   */
  void set_asset_root(const std::string& path);

  std::string asset_root_path_ = "";
};

}  // namespace engine::graphics

#endif  // INCLUDE_GRAPHICS_RENDERER_H_
