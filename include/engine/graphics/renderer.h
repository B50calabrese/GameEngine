/**
 * @file renderer.h
 * @brief Core rendering API.
 */

#ifndef INCLUDE_ENGINE_GRAPHICS_RENDERER_H_
#define INCLUDE_ENGINE_GRAPHICS_RENDERER_H_

#include <string>

#include <glm/glm.hpp>

#include <engine/core/window.h>
#include <engine/graphics/camera.h>

namespace engine {
class Engine;
}

namespace engine::graphics {

class Texture;
class SpriteSheet;

/**
 * @brief Manages the graphics context and provides the core drawing API.
 */
class Renderer {
 public:
  /** @brief Gets the singleton instance. */
  static Renderer& Get() {
    static Renderer instance;
    return instance;
  }

  /** @brief Clears the screen. */
  void Clear() const;

  /** @brief Prepares for a new frame. */
  void BeginFrame(Camera& camera) const;

  /** @brief Finalizes the frame. */
  void EndFrame() const;

  /** @brief Flushes batched geometry to the GPU. */
  void Flush() const;

  /** @brief Draws a solid rectangle. */
  void DrawRect(float x, float y, float width, float height,
                const glm::vec4& color = glm::vec4(1.0f));

  /** @brief Draws a solid colored rectangle using RGB values. */
  void DrawRect(float x, float y, float width, float height, float r, float g,
                float b);

  /** @brief Draws a colored quad with full transformation support. */
  void DrawQuad(const glm::vec2& position, const glm::vec2& size,
                const glm::vec4& color, float rotation = 0.0f,
                const glm::vec2& origin = {0.0f, 0.0f});

  /** @brief Draws a textured quad. */
  void DrawTexturedQuad(const glm::vec2& position, const glm::vec2& size,
                        unsigned int texture_id, float rotation = 0.0f,
                        const glm::vec4& tint = glm::vec4(1.0f),
                        const glm::vec2& origin = {0.0f, 0.0f});

  /** @brief Draws a textured quad using a Texture resource. */
  void DrawTexturedQuad(const glm::vec2& position, const glm::vec2& size,
                        const Texture* texture, float rotation = 0.0f,
                        const glm::vec4& tint = glm::vec4(1.0f),
                        const glm::vec2& origin = {0.0f, 0.0f});

  /** @brief Draws a sprite from a sprite sheet. */
  void DrawSprite(const SpriteSheet* sprite_sheet, int index,
                  const glm::vec2& position, const glm::vec2& size,
                  float rotation = 0.0f,
                  const glm::vec4& tint = glm::vec4(1.0f),
                  const glm::vec2& origin = {0.0f, 0.0f});

  /** @brief Renders text. */
  void DrawText(const std::string& font_name, const std::string& text,
                const glm::vec2& position, float rotation = 0.0f,
                float scale = 1.0f, const glm::vec4& color = glm::vec4(1.0f),
                float z_index = 0.0f);

  /** @brief Resolves a relative asset path. */
  std::string ResolveAssetPath(const std::string& relative_path) const;

  /** @brief Handles window resize. */
  void HandleResize(int width, int height);

 private:
  Renderer() = default;
  ~Renderer() = default;

  friend class engine::Engine;

  Renderer(const Renderer&) = delete;
  Renderer& operator=(const Renderer&) = delete;

  /** @brief Initializes the OpenGL context. */
  void Init(Window& window);

  /** @brief Cleans up graphics resources. */
  void Shutdown();

  /** @brief Sets the OpenGL viewport. */
  void SetViewport(int width, int height) const;

  /** @brief Sets the asset root path. */
  void SetAssetRoot(const std::string& path);

  std::string asset_root_path_;
};

}  // namespace engine::graphics

#endif  // INCLUDE_ENGINE_GRAPHICS_RENDERER_H_
