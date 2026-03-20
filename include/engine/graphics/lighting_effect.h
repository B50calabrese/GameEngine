/**
 * @file lighting_effect.h
 * @brief 2D Lighting post-processing effect.
 */

#ifndef INCLUDE_ENGINE_GRAPHICS_LIGHTING_EFFECT_H_
#define INCLUDE_ENGINE_GRAPHICS_LIGHTING_EFFECT_H_

#include <vector>
#include <string>

#include <glm/glm.hpp>
#include <engine/graphics/post_processor.h>
#include <engine/graphics/framebuffer.h>
#include <engine/graphics/shader.h>

namespace engine::graphics {

enum class ShadowType {
  Raymarch,
  Polar
};

struct Light {
  glm::vec2 position;
  glm::vec3 color = {1.0f, 1.0f, 1.0f};
  float intensity = 1.0f;
  float radius = 100.0f;

  // For Spotlights
  float angle = 360.0f; // 360 = point light
  float direction = 0.0f;

  bool is_directional = false;
  glm::vec2 dir_vector = {0.0f, -1.0f};
};

struct Occluder {
  glm::vec2 position;
  glm::vec2 size;
  float rotation = 0.0f;
};

class LightingEffect : public IPostProcessEffect {
 public:
  LightingEffect();
  ~LightingEffect() override;

  void OnResize(int width, int height) override;
  void Apply(unsigned int input_texture, Framebuffer* output_framebuffer) override;
  std::string GetName() const override { return "LightingEffect"; }

  void AddLight(const Light& light) { lights_.push_back(light); }
  void ClearLights() { lights_.clear(); }

  void AddOccluder(const Occluder& occluder) { occluders_.push_back(occluder); }
  void ClearOccluders() { occluders_.clear(); }

  void SetAmbientLight(const glm::vec3& color, float intensity) {
    ambient_color_ = color;
    ambient_intensity_ = intensity;
  }

  void SetShadowType(ShadowType type) { shadow_type_ = type; }
  void SetShadowBlur(float blur) { shadow_blur_ = blur; }
  void SetUseMultiplicativeBlending(bool use) { multiplicative_ = use; }

 private:
  void RenderOccluderMap();
  void InitResources();

  std::vector<Light> lights_;
  std::vector<Occluder> occluders_;

  std::shared_ptr<Shader> lighting_shader_;
  std::shared_ptr<Shader> occluder_shader_;

  std::unique_ptr<Framebuffer> occluder_map_;

  glm::vec3 ambient_color_ = {0.1f, 0.1f, 0.1f};
  float ambient_intensity_ = 0.5f;

  ShadowType shadow_type_ = ShadowType::Raymarch;
  float shadow_blur_ = 0.05f;
  bool multiplicative_ = true;

  unsigned int quad_vao_ = 0;
  unsigned int quad_vbo_ = 0;

  int width_ = 0;
  int height_ = 0;
};

} // namespace engine::graphics

#endif // INCLUDE_ENGINE_GRAPHICS_LIGHTING_EFFECT_H_
