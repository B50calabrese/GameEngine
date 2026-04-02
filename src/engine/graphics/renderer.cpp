/**
 * @file renderer.cpp
 * @brief Renderer class implementation.
 */

#include <engine/graphics/renderer.h>

// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

#include <filesystem>

#include <engine/core/job_system.h>
#include <engine/graphics/camera.h>
#include <engine/graphics/post_processor.h>
#include <engine/graphics/primitive_renderer.h>
#include <engine/graphics/sprite_sheet.h>
#include <engine/graphics/text_renderer.h>
#include <engine/graphics/texture.h>
#include <engine/util/logger.h>

namespace engine::graphics {

void Renderer::Clear() const {
  ASSERT_MAIN_THREAD();
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::BeginFrame(Camera& camera) const {
  ASSERT_MAIN_THREAD();
  PostProcessManager::Get().Begin();
  PrimitiveRenderer::StartBatch(camera.GetViewProjectionMatrix());
}

void Renderer::EndFrame() const {
  ASSERT_MAIN_THREAD();
  Flush();
  PostProcessManager::Get().End();
}

void Renderer::Flush() const {
  ASSERT_MAIN_THREAD();
  PrimitiveRenderer::FinalizeBatch();
  PrimitiveRenderer::RenderBatch();
}

void Renderer::DrawRect(float x, float y, float width, float height,
                        const glm::vec4& color) {
  ASSERT_MAIN_THREAD();
  PrimitiveRenderer::SubmitQuad({x, y}, {width, height}, color);
}

void Renderer::DrawRect(float x, float y, float width, float height, float r,
                        float g, float b) {
  ASSERT_MAIN_THREAD();
  PrimitiveRenderer::SubmitQuad({x, y}, {width, height}, {r, g, b, 1.0f});
}

void Renderer::DrawQuad(const glm::vec2& position, const glm::vec2& size,
                        const glm::vec4& color, float rotation,
                        const glm::vec2& origin) {
  ASSERT_MAIN_THREAD();
  PrimitiveRenderer::SubmitQuad(position, size, color, rotation, origin);
}

void Renderer::DrawTexturedQuad(const glm::vec2& position,
                                const glm::vec2& size, unsigned int texture_id,
                                float rotation, const glm::vec4& tint,
                                const glm::vec2& origin) {
  ASSERT_MAIN_THREAD();
  PrimitiveRenderer::SubmitTexturedQuad(position, size, texture_id, tint,
                                        rotation, origin);
}

void Renderer::DrawTexturedQuad(const glm::vec2& position,
                                const glm::vec2& size, const Texture* texture,
                                float rotation, const glm::vec4& tint,
                                const glm::vec2& origin) {
  ASSERT_MAIN_THREAD();
  if (texture) {
    PrimitiveRenderer::SubmitTexturedQuad(
        position, size, texture->renderer_id(), tint, rotation, origin);
  }
}

void Renderer::DrawSprite(const SpriteSheet* sprite_sheet, int index,
                          const glm::vec2& position, const glm::vec2& size,
                          float rotation, const glm::vec4& tint,
                          const glm::vec2& origin) {
  ASSERT_MAIN_THREAD();
  if (sprite_sheet && sprite_sheet->texture()) {
    glm::vec2 uv_min, uv_max;
    sprite_sheet->GetUVs(index, &uv_min, &uv_max);
    PrimitiveRenderer::SubmitTexturedQuad(
        position, size, sprite_sheet->texture()->renderer_id(), uv_min, uv_max,
        tint, rotation, origin);
  }
}

void Renderer::DrawText(const std::string& font_name, const std::string& text,
                        const glm::vec2& position, float rotation, float scale,
                        const glm::vec4& color, float z_index) {
  ASSERT_MAIN_THREAD();
  TextRenderer::Get().DrawText(font_name, text, position, rotation, scale,
                               color, z_index);
}

std::string Renderer::ResolveAssetPath(const std::string& relative_path) const {
  std::filesystem::path p(relative_path);
  if (p.is_absolute()) {
    return relative_path;
  }
  if (asset_root_path_.empty()) {
    return relative_path;
  }
  return asset_root_path_ + relative_path;
}

void Renderer::Init(Window& window) {
  if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
    LOG_ERR("Failed to initialize GLAD");
    return;
  }

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glDisable(GL_DEPTH_TEST);

  SetViewport(window.GetWidth(), window.GetHeight());

  PrimitiveRenderer::Init();
  PostProcessManager::Get().Init(window.GetWidth(), window.GetHeight());
}

void Renderer::Shutdown() { PrimitiveRenderer::Shutdown(); }

void Renderer::SetViewport(int width, int height) const {
  glViewport(0, 0, width, height);
}

void Renderer::HandleResize(int width, int height) {
  SetViewport(width, height);
  PostProcessManager::Get().OnResize(width, height);
}

void Renderer::SetAssetRoot(const std::string& path) {
  if (path.empty()) {
    asset_root_path_ = "";
    return;
  }
  asset_root_path_ = std::filesystem::absolute(path).string();
  if (!asset_root_path_.empty() && asset_root_path_.back() != '/' &&
      asset_root_path_.back() != '\\') {
    asset_root_path_ += "/";
  }
}

}  // namespace engine::graphics
