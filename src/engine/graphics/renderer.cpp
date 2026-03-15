/**
 * @file renderer.cpp
 * @brief Renderer class implementation.
 */

/**
 * @dir src/engine/graphics
 * @brief Rendering system internals.
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
  // Instruct renders to reset themselves for the frame.
  graphics::PrimitiveRenderer::StartBatch(camera.view_projection_matrix());
}

void Renderer::EndFrame() const {
  ASSERT_MAIN_THREAD();
  // Flush all renderers.
  Flush();
  PostProcessManager::Get().End();
}

void Renderer::Flush() const {
  ASSERT_MAIN_THREAD();
  graphics::PrimitiveRenderer::FinalizeBatch();
  graphics::PrimitiveRenderer::RenderBatch();
}

void Renderer::DrawRect(float x, float y, float width, float height) {
  ASSERT_MAIN_THREAD();
  // Default white color for basic DrawRect call
  glm::vec4 color(1.0f, 1.0f, 1.0f, 1.0f);
  PrimitiveRenderer::SubmitQuad({x, y}, {width, height}, color);
}

void Renderer::DrawRect(float x, float y, float width, float height, float r,
                        float g, float b) {
  ASSERT_MAIN_THREAD();
  glm::vec4 color(r, g, b, 1.0f);
  PrimitiveRenderer::SubmitQuad({x, y}, {width, height}, color);
}

void Renderer::DrawQuad(const glm::vec2& position, const glm::vec2& size,
                        const glm::vec4& color, float rotation,
                        const glm::vec2& origin) {
  ASSERT_MAIN_THREAD();
  PrimitiveRenderer::SubmitQuad(position, size, color, rotation, origin);
}

void Renderer::DrawTexturedRect(float x, float y, float w, float h,
                                unsigned int texture_id, const float tint[4]) {
  ASSERT_MAIN_THREAD();
  glm::vec4 color(1.0f, 1.0f, 1.0f, 1.0f);
  if (tint) {
    color = glm::vec4(tint[0], tint[1], tint[2], tint[3]);
  }

  PrimitiveRenderer::SubmitTexturedQuad({x, y}, {w, h}, texture_id, color);
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

void Renderer::DrawSprite(const class SpriteSheet* sprite_sheet, int index,
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
                        const glm::vec4& color) {
  ASSERT_MAIN_THREAD();
  TextRenderer::Get().DrawText(font_name, text, position, rotation, scale,
                               color);
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

// Retrieve the native handle from the window
void Renderer::Init(Window& window) {
  // Load Glad
  if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
    LOG_ERR("Failed to initialize GLAD");
    return;
  }

  // Set default OpenGL state.
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // Note we disable the depth test for 2D games. If we don't then the painter's
  // algorithm doesn't work.
  // As a future improvement we could make this something we automatically
  // enable and disable per frame for different parts of the game.
  glDisable(GL_DEPTH_TEST);

  // Set viewport to window dimensions
  int width, height;
  glfwGetWindowSize(window.native_handle(), &width, &height);
  set_viewport(width, height);

  // Initialize renderers.
  graphics::PrimitiveRenderer::Init();
  PostProcessManager::Get().Init(window.width(), window.height());
}

void Renderer::Shutdown() {
  // Shutdown renderers.
  graphics::PrimitiveRenderer::Shutdown();
}

void Renderer::set_viewport(int width, int height) const {
  glViewport(0, 0, width, height);
}

void Renderer::HandleResize(int width, int height) {
  this->set_viewport(width, height);
  PostProcessManager::Get().OnResize(width, height);
}

void Renderer::set_asset_root(const std::string& path) {
  if (path.empty()) {
    asset_root_path_ = "";
    return;
  }
  asset_root_path_ = std::filesystem::absolute(path).string();
  // Ensure trailing slash for consistent concatenation
  if (!asset_root_path_.empty() && asset_root_path_.back() != '/' &&
      asset_root_path_.back() != '\\') {
    asset_root_path_ += "/";
  }
}
}  // namespace engine::graphics
