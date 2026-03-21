/**
 * @file post_processor.h
 * @brief Manages full-screen post-processing effects.
 */

#ifndef INCLUDE_ENGINE_GRAPHICS_POST_PROCESSOR_H_
#define INCLUDE_ENGINE_GRAPHICS_POST_PROCESSOR_H_

#include <memory>
#include <string>
#include <vector>

#include <glm/glm.hpp>

#include <engine/graphics/framebuffer.h>
#include <engine/graphics/shader.h>

namespace engine::graphics {

/**
 * @brief Interface for a post-processing effect.
 */
class IPostProcessEffect {
 public:
  virtual ~IPostProcessEffect() = default;

  /**
   * @brief Called when the effect is initialized or the window is resized.
   * @param width New width.
   * @param height New height.
   */
  virtual void OnResize(int width, int height) = 0;

  /**
   * @brief Applies the effect.
   * @param input_texture The texture to process.
   * @param output_framebuffer The framebuffer to render into.
   */
  virtual void Apply(unsigned int input_texture,
                     Framebuffer* output_framebuffer) = 0;

  /** @brief Returns the name of the effect. */
  virtual std::string GetName() const = 0;
};

/**
 * @brief Standard post-processing effect (shake, flash, palette).
 * Maintains backward compatibility with existing engine features.
 */
class StandardEffect : public IPostProcessEffect {
 public:
  StandardEffect();
  ~StandardEffect() override;

  void OnResize(int width, int height) override {}
  void Apply(unsigned int input_texture,
             Framebuffer* output_framebuffer) override;
  std::string GetName() const override { return "StandardEffect"; }

  void SetShake(float intensity) { shake_intensity_ = intensity; }
  void SetFlash(const glm::vec3& color, float amount) {
    flash_color_ = color;
    flash_amount_ = amount;
  }
  void SetPalette(unsigned int texture_id) { palette_tex_id_ = texture_id; }

 private:
  void InitQuad();

  std::shared_ptr<Shader> post_shader_;
  unsigned int quad_vao_ = 0;
  unsigned int quad_vbo_ = 0;

  float shake_intensity_ = 0.0f;
  glm::vec3 flash_color_ = {1.0f, 1.0f, 1.0f};
  float flash_amount_ = 0.0f;
  unsigned int palette_tex_id_ = 0;
};

/**
 * @brief Singleton manager for post-processing effects.
 *
 * Handles the transition between scene rendering and post-process passes.
 */
class PostProcessManager {
 public:
  /**
   * @brief Returns a reference to the singleton `PostProcessManager` instance.
   * @return Reference to the PostProcessManager.
   */
  static PostProcessManager& Get() {
    static PostProcessManager instance;
    return instance;
  }

  /**
   * @brief Initializes the manager with given dimensions.
   * @param width Initial width.
   * @param height Initial height.
   */
  void Init(int width, int height);

  /** @brief Prepares the FBO for scene rendering. */
  void Begin();

  /** @brief Finalizes scene and renders the post-process pipeline to screen. */
  void End();

  /**
   * @brief Notifies the manager of a window resize.
   * @param width New width.
   * @param height New height.
   */
  void OnResize(int width, int height);

  /**
   * @brief Adds an effect to the end of the pipeline.
   * @param effect The effect to add.
   */
  void AddEffect(std::unique_ptr<IPostProcessEffect> effect);

  /**
   * @brief Removes all effects from the pipeline.
   */
  void ClearEffects();

  // Backward compatibility methods for StandardEffect
  void SetShake(float intensity);
  void SetFlash(const glm::vec3& color, float amount);
  void SetPalette(unsigned int texture_id);

 private:
  PostProcessManager() = default;
  ~PostProcessManager() = default;

  // Prevent copy/move
  PostProcessManager(const PostProcessManager&) = delete;
  PostProcessManager& operator=(const PostProcessManager&) = delete;

  std::unique_ptr<Framebuffer> scene_buffer_;
  std::unique_ptr<Framebuffer> ping_pong_buffer_;

  std::vector<std::unique_ptr<IPostProcessEffect>> effects_;
  StandardEffect* standard_effect_ =
      nullptr;  // Raw pointer to the effect in the vector for quick access

  int width_ = 0;
  int height_ = 0;
};

}  // namespace engine::graphics

#endif  // INCLUDE_ENGINE_GRAPHICS_POST_PROCESSOR_H_
