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

PostProcessManager::~PostProcessManager() {
  if (quad_vao_) {
    glDeleteVertexArrays(1, &quad_vao_);
  }
  if (quad_vbo_) {
    glDeleteBuffers(1, &quad_vbo_);
  }
}

void PostProcessManager::Init(int width, int height) {
  width_ = width;
  height_ = height;
  scene_buffer_ = std::make_unique<Framebuffer>(width, height);
  post_shader_ = Shader::CreateFromSource(kPostProcessVertexSource,
                                          kPostProcessFragmentSource);
  InitQuad();
}

void PostProcessManager::Begin() {
  if (scene_buffer_) {
    scene_buffer_->Bind();
  }
}

void PostProcessManager::End() {
  if (!scene_buffer_ || !post_shader_) {
    return;
  }

  scene_buffer_->Unbind();

  // Disable depth test to ensure the quad is always rendered
  glDisable(GL_DEPTH_TEST);

  // Draw the full-screen quad with post-process shader
  post_shader_->Bind();
  post_shader_->SetInt("u_ScreenTexture", 0);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, scene_buffer_->texture_id());

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

void PostProcessManager::OnResize(int width, int height) {
  width_ = width;
  height_ = height;
  if (scene_buffer_) {
    scene_buffer_->Resize(width, height);
  }
}

void PostProcessManager::InitQuad() {
  float quadVertices[] = {// positions   // texCoords
                          -1.0f, 1.0f, 0.0f, 1.0f,  -1.0f, -1.0f,
                          0.0f,  0.0f, 1.0f, -1.0f, 1.0f,  0.0f,

                          -1.0f, 1.0f, 0.0f, 1.0f,  1.0f,  -1.0f,
                          1.0f,  0.0f, 1.0f, 1.0f,  1.0f,  1.0f};

  glGenVertexArrays(1, &quad_vao_);
  glGenBuffers(1, &quad_vbo_);
  glBindVertexArray(quad_vao_);
  glBindBuffer(GL_ARRAY_BUFFER, quad_vbo_);
  glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices,
               GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                        (void*)(2 * sizeof(float)));
}

}  // namespace engine::graphics
