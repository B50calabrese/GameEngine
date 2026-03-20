/**
 * @file post_processor.cpp
 * @brief PostProcessManager implementation.
 */

// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

#include <engine/graphics/post_processor.h>
#include <engine/util/logger.h>

namespace engine::graphics {

static const char* kPostProcessVertexSource = R"(
    #version 330 core
    layout (location = 0) in vec2 aPos;
    layout (location = 1) in vec2 aTexCoords;

    out vec2 TexCoords;

    void main() {
        gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
        TexCoords = aTexCoords;
    }
)";

static const char* kPostProcessFragmentSource = R"(
    #version 330 core
    out vec4 FragColor;

    in vec2 TexCoords;

    uniform sampler2D u_ScreenTexture;
    uniform sampler2D u_PaletteTexture;
    uniform float u_ShakeIntensity;
    uniform float u_FlashAmount;
    uniform vec3 u_FlashColor;
    uniform float u_Time;
    uniform bool u_UsePalette;

    void main() {
        // 1. UV Shake
        vec2 uv = TexCoords;
        if (u_ShakeIntensity > 0.0) {
            uv.x += cos(u_Time * 50.0) * u_ShakeIntensity;
            uv.y += sin(u_Time * 50.0) * u_ShakeIntensity;
        }

        vec4 color = texture(u_ScreenTexture, uv);

        // 2. Palette Swapping (using grayscale as index)
        if (u_UsePalette) {
            float grayscale = dot(color.rgb, vec3(0.299, 0.587, 0.114));
            color = texture(u_PaletteTexture, vec2(grayscale, 0.5));
        }

        // 3. Flash Overlay
        color.rgb = mix(color.rgb, u_FlashColor, u_FlashAmount);

        FragColor = color;
    }
)";

StandardEffect::StandardEffect() {
  post_shader_ = Shader::CreateFromSource(kPostProcessVertexSource,
                                          kPostProcessFragmentSource);
  InitQuad();
}

StandardEffect::~StandardEffect() {
  if (quad_vao_) glDeleteVertexArrays(1, &quad_vao_);
  if (quad_vbo_) glDeleteBuffers(1, &quad_vbo_);
}

void StandardEffect::Apply(unsigned int input_texture, Framebuffer* output_framebuffer) {
  if (output_framebuffer) {
    output_framebuffer->Bind();
  } else {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }

  glDisable(GL_DEPTH_TEST);
  post_shader_->Bind();
  post_shader_->SetInt("u_ScreenTexture", 0);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, input_texture);

  if (palette_tex_id_ != 0) {
    post_shader_->SetInt("u_PaletteTexture", 1);
    post_shader_->SetInt("u_UsePalette", 1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, palette_tex_id_);
  } else {
    post_shader_->SetInt("u_UsePalette", 0);
  }

  post_shader_->SetFloat("u_ShakeIntensity", shake_intensity_);
  post_shader_->SetVec3("u_FlashColor", flash_color_);
  post_shader_->SetFloat("u_FlashAmount", flash_amount_);
  post_shader_->SetFloat("u_Time", static_cast<float>(glfwGetTime()));

  glBindVertexArray(quad_vao_);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glBindVertexArray(0);

  post_shader_->Unbind();
}

void StandardEffect::InitQuad() {
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

void PostProcessManager::Init(int width, int height) {
  width_ = width;
  height_ = height;
  scene_buffer_ = std::make_unique<Framebuffer>(width, height);
  ping_pong_buffer_ = std::make_unique<Framebuffer>(width, height);

  // Initialize with the standard effect for backward compatibility
  auto standard = std::make_unique<StandardEffect>();
  standard_effect_ = standard.get();
  AddEffect(std::move(standard));
}

void PostProcessManager::Begin() {
  if (scene_buffer_) {
    scene_buffer_->Bind();
  }
}

void PostProcessManager::End() {
  if (!scene_buffer_ || effects_.empty()) {
    return;
  }

  unsigned int current_input = scene_buffer_->texture_id();
  Framebuffer* current_output = ping_pong_buffer_.get();

  for (size_t i = 0; i < effects_.size(); ++i) {
    bool last_effect = (i == effects_.size() - 1);
    Framebuffer* output = last_effect ? nullptr : current_output;

    effects_[i]->Apply(current_input, output);

    if (!last_effect) {
      current_input = current_output->texture_id();
      // Swap ping-pong buffer if we have more effects
      if (current_output == ping_pong_buffer_.get()) {
        // We'd need another buffer for more than 2 effects if we don't want to re-render to the same ones.
        // For simplicity, let's assume we can re-use scene_buffer_ as a ping-pong buffer.
        current_output = scene_buffer_.get();
      } else {
        current_output = ping_pong_buffer_.get();
      }
    }
  }
}

void PostProcessManager::OnResize(int width, int height) {
  width_ = width;
  height_ = height;
  if (scene_buffer_) scene_buffer_->Resize(width, height);
  if (ping_pong_buffer_) ping_pong_buffer_->Resize(width, height);
  for (auto& effect : effects_) {
    effect->OnResize(width, height);
  }
}

void PostProcessManager::AddEffect(std::unique_ptr<IPostProcessEffect> effect) {
  effect->OnResize(width_, height_);
  effects_.push_back(std::move(effect));
}

void PostProcessManager::ClearEffects() {
  effects_.clear();
  standard_effect_ = nullptr;
}

void PostProcessManager::SetShake(float intensity) {
  if (standard_effect_) standard_effect_->SetShake(intensity);
}

void PostProcessManager::SetFlash(const glm::vec3& color, float amount) {
  if (standard_effect_) standard_effect_->SetFlash(color, amount);
}

void PostProcessManager::SetPalette(unsigned int texture_id) {
  if (standard_effect_) standard_effect_->SetPalette(texture_id);
}

}  // namespace engine::graphics
