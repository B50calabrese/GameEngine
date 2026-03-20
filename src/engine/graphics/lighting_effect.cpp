/**
 * @file lighting_effect.cpp
 * @brief 2D Lighting post-processing effect implementation.
 */

// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

#include <engine/graphics/lighting_effect.h>
#include <engine/util/logger.h>
#include <glm/gtc/type_ptr.hpp>

namespace engine::graphics {

static const char* kOccluderVertexSource = R"(
    #version 330 core
    layout (location = 0) in vec2 aPos;
    layout (location = 1) in vec2 aTexCoords;

    uniform mat4 u_Model;
    uniform mat4 u_Projection;

    void main() {
        gl_Position = u_Projection * u_Model * vec4(aPos, 0.0, 1.0);
    }
)";

static const char* kOccluderFragmentSource = R"(
    #version 330 core
    out vec4 FragColor;

    void main() {
        FragColor = vec4(1.0, 1.0, 1.0, 1.0);
    }
)";

static const char* kLightingVertexSource = R"(
    #version 330 core
    layout (location = 0) in vec2 aPos;
    layout (location = 1) in vec2 aTexCoords;

    out vec2 TexCoords;

    void main() {
        gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
        TexCoords = aTexCoords;
    }
)";

static const char* kLightingFragmentSource = R"(
    #version 330 core
    out vec4 FragColor;

    in vec2 TexCoords;

    uniform sampler2D u_ScreenTexture;
    uniform sampler2D u_OccluderMap;

    struct Light {
        vec2 position;
        vec3 color;
        float intensity;
        float radius;
        float angle;
        float direction;
        bool is_directional;
        vec2 dir_vector;
    };

    #define MAX_LIGHTS 16
    uniform Light u_Lights[MAX_LIGHTS];
    uniform int u_NumLights;

    uniform vec3 u_AmbientColor;
    uniform float u_AmbientIntensity;
    uniform float u_ShadowBlur;
    uniform bool u_Multiplicative;
    uniform int u_ShadowType; // 0 = Raymarch, 1 = Polar
    uniform vec2 u_Resolution;

    float sample_occlusion(vec2 uv) {
        return texture(u_OccluderMap, uv).r;
    }

    float calculate_raymarch_shadow(vec2 light_pos, vec2 current_pos) {
        vec2 dir = normalize(light_pos - current_pos);
        float dist = distance(light_pos * u_Resolution, current_pos * u_Resolution);

        // Raymarching for shadows
        const int steps = 64;
        float step_size = dist / float(steps);
        float shadow = 1.0;

        for (int i = 1; i < steps; i++) {
            vec2 sample_pos_pixels = current_pos * u_Resolution + dir * step_size * float(i);
            vec2 sample_uv = sample_pos_pixels / u_Resolution;
            if (sample_occlusion(sample_uv) > 0.5) {
                // If u_ShadowBlur > 0, we can soften by not returning 0 immediately
                if (u_ShadowBlur <= 0.0) return 0.0;

                shadow -= (1.0 / float(steps)) * (1.0 / u_ShadowBlur);
                if (shadow <= 0.0) return 0.0;
            }
        }
        return shadow;
    }

    // A simpler multi-sample shadow for "Polar-like" or just soft shadows
    float calculate_soft_shadow(vec2 light_pos, vec2 current_pos) {
        float shadow = 0.0;
        const int num_samples = 8;
        float blur_radius = u_ShadowBlur * 10.0;

        for (int i = 0; i < num_samples; i++) {
            float angle = 6.28318 * float(i) / float(num_samples);
            vec2 offset = vec2(cos(angle), sin(angle)) * blur_radius / u_Resolution;
            shadow += calculate_raymarch_shadow(light_pos, current_pos + offset);
        }
        return shadow / float(num_samples);
    }

    void main() {
        vec4 base_color = texture(u_ScreenTexture, TexCoords);
        vec3 total_light = u_AmbientColor * u_AmbientIntensity;

        for (int i = 0; i < u_NumLights; i++) {
            Light light = u_Lights[i];
            vec3 light_contrib = vec3(0.0);

            if (light.is_directional) {
                // Directional Light
                float diff = max(dot(normalize(-light.dir_vector), vec2(0, 1)), 0.0); // Simple 2D top-down diff
                // For 2D directional light, we often just want a global tint/intensity
                float shadow = 1.0;
                if (u_ShadowType == 0) {
                   // Raymarch shadow from far away in dir_vector direction
                   shadow = calculate_raymarch_shadow(TexCoords - light.dir_vector * 1.0, TexCoords);
                }
                light_contrib = light.color * light.intensity * shadow;
            } else {
                // Point/Spot Light
                vec2 light_uv = light.position;
                float pixel_dist = distance(TexCoords * u_Resolution, light_uv * u_Resolution);

                if (pixel_dist < light.radius) {
                    float attenuation = 1.0 - (pixel_dist / light.radius);
                    attenuation *= light.intensity;

                    // Spotlight cone
                    if (light.angle < 360.0) {
                        vec2 to_pixel = normalize(TexCoords - light_uv);
                        vec2 dir = normalize(light.dir_vector);
                        float theta = acos(dot(to_pixel, dir));
                        float epsilon = radians(light.angle) * 0.5;
                        if (theta > epsilon) {
                            attenuation = 0.0;
                        } else {
                            // Smooth edge
                            attenuation *= smoothstep(epsilon, epsilon * 0.8, theta);
                        }
                    }

                    if (attenuation > 0.0) {
                        float shadow = 1.0;
                        if (u_ShadowType == 0) {
                            shadow = calculate_raymarch_shadow(light_uv, TexCoords);
                        } else if (u_ShadowType == 1) {
                            shadow = calculate_soft_shadow(light_uv, TexCoords);
                        }
                        light_contrib = light.color * attenuation * shadow;
                    }
                }
            }
            total_light += light_contrib;
        }

        if (u_Multiplicative) {
            FragColor = vec4(base_color.rgb * total_light, base_color.a);
        } else {
            FragColor = vec4(base_color.rgb + total_light * 0.5, base_color.a);
        }
    }
)";

LightingEffect::LightingEffect() {
  InitResources();
}

LightingEffect::~LightingEffect() {
  if (quad_vao_) glDeleteVertexArrays(1, &quad_vao_);
  if (quad_vbo_) glDeleteBuffers(1, &quad_vbo_);
}

void LightingEffect::InitResources() {
  lighting_shader_ = Shader::CreateFromSource(kLightingVertexSource, kLightingFragmentSource);
  occluder_shader_ = Shader::CreateFromSource(kOccluderVertexSource, kOccluderFragmentSource);

  float quadVertices[] = {
      -1.0f, 1.0f,  0.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, -1.0f, 1.0f, 0.0f,
      -1.0f, 1.0f,  0.0f, 1.0f, 1.0f,  -1.0f, 1.0f, 0.0f, 1.0f, 1.0f,  1.0f, 1.0f};

  glGenVertexArrays(1, &quad_vao_);
  glGenBuffers(1, &quad_vbo_);
  glBindVertexArray(quad_vao_);
  glBindBuffer(GL_ARRAY_BUFFER, quad_vbo_);
  glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
}

void LightingEffect::OnResize(int width, int height) {
  width_ = width;
  height_ = height;
  occluder_map_ = std::make_unique<Framebuffer>(width, height);
}

void LightingEffect::RenderOccluderMap() {
  if (!occluder_map_ || !occluder_shader_) return;

  occluder_map_->Bind();
  glViewport(0, 0, width_, height_);
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  occluder_shader_->Bind();
  glm::mat4 projection = glm::ortho(0.0f, (float)width_, 0.0f, (float)height_);
  occluder_shader_->SetMat4("u_Projection", projection);

  // We need a simple quad for drawing occluders
  // For now let's just use the shared quad if it was generic, but here we need world-space quads.
  // We'll reuse the quad_vao but scale/translate it via u_Model.
  glBindVertexArray(quad_vao_);

  for (const auto& occluder : occluders_) {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(occluder.position, 0.0f));
    model = glm::rotate(model, glm::radians(occluder.rotation), glm::vec3(0, 0, 1));
    model = glm::scale(model, glm::vec3(occluder.size * 0.5f, 1.0f)); // 0.5 because our quad is -1 to 1

    occluder_shader_->SetMat4("u_Model", model);
    glDrawArrays(GL_TRIANGLES, 0, 6);
  }

  glBindVertexArray(0);
  occluder_map_->Unbind();
}

void LightingEffect::Apply(unsigned int input_texture, Framebuffer* output_framebuffer) {
  RenderOccluderMap();

  if (output_framebuffer) {
    output_framebuffer->Bind();
    glViewport(0, 0, output_framebuffer->width(), output_framebuffer->height());
  } else {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, width_, height_);
  }

  glDisable(GL_DEPTH_TEST);
  lighting_shader_->Bind();

  lighting_shader_->SetInt("u_ScreenTexture", 0);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, input_texture);

  lighting_shader_->SetInt("u_OccluderMap", 1);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, occluder_map_->texture_id());

  lighting_shader_->SetVec3("u_AmbientColor", ambient_color_);
  lighting_shader_->SetFloat("u_AmbientIntensity", ambient_intensity_);
  lighting_shader_->SetFloat("u_ShadowBlur", shadow_blur_);
  lighting_shader_->SetInt("u_Multiplicative", multiplicative_ ? 1 : 0);
  lighting_shader_->SetInt("u_ShadowType", (int)shadow_type_);
  lighting_shader_->SetVec2("u_Resolution", glm::vec2(width_, height_));

  int num_lights = std::min((int)lights_.size(), 16);
  lighting_shader_->SetInt("u_NumLights", num_lights);

  for (int i = 0; i < num_lights; i++) {
    std::string prefix = "u_Lights[" + std::to_string(i) + "].";
    // Convert light position to UV space [0, 1]
    glm::vec2 uv_pos = lights_[i].position / glm::vec2(width_, height_);
    lighting_shader_->SetVec2(prefix + "position", uv_pos);
    lighting_shader_->SetVec3(prefix + "color", lights_[i].color);
    lighting_shader_->SetFloat(prefix + "intensity", lights_[i].intensity);
    lighting_shader_->SetFloat(prefix + "radius", lights_[i].radius);
      lighting_shader_->SetFloat(prefix + "angle", lights_[i].angle);
      lighting_shader_->SetFloat(prefix + "direction", lights_[i].direction);
      lighting_shader_->SetInt(prefix + "is_directional", lights_[i].is_directional ? 1 : 0);
      lighting_shader_->SetVec2(prefix + "dir_vector", lights_[i].dir_vector);
  }

  glBindVertexArray(quad_vao_);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glBindVertexArray(0);

  lighting_shader_->Unbind();
}

} // namespace engine::graphics
