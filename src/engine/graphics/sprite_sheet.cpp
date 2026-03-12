/**
 * @file sprite_sheet.cpp
 * @brief SpriteSheet implementation.
 */

#include <engine/graphics/sprite_sheet.h>

#include <sstream>
#include <vector>

#include <engine/graphics/texture.h>
#include <engine/util/logger.h>

namespace engine::graphics {

std::shared_ptr<SpriteSheet> SpriteSheet::Load(const std::string& path) {
  std::stringstream ss(path);
  std::string texture_path;
  std::string segment;
  std::vector<std::string> segments;

  while (std::getline(ss, segment, ':')) {
    segments.push_back(segment);
  }

  if (segments.size() != 5) {
    LOG_ERR("Invalid SpriteSheet load path format: %s. Expected "
            "'path:w:h:rows:cols'",
            path.c_str());
    return nullptr;
  }

  texture_path = segments[0];
  int w = std::stoi(segments[1]);
  int h = std::stoi(segments[2]);
  int rows = std::stoi(segments[3]);
  int cols = std::stoi(segments[4]);

  auto texture = Texture::Load(texture_path);
  if (!texture) {
    return nullptr;
  }

  return Create(texture, w, h, rows, cols);
}

std::shared_ptr<SpriteSheet> SpriteSheet::Create(
    std::shared_ptr<Texture> texture, int sprite_width, int sprite_height,
    int rows, int cols) {
  return std::shared_ptr<SpriteSheet>(
      new SpriteSheet(texture, sprite_width, sprite_height, rows, cols));
}

void SpriteSheet::GetUVs(int index, glm::vec2* uv_min,
                         glm::vec2* uv_max) const {
  if (!texture_ || index < 0 || index >= (rows_ * cols_)) {
    if (uv_min) {
      *uv_min = {0.0f, 0.0f};
    }
    if (uv_max) {
      *uv_max = {1.0f, 1.0f};
    }
    return;
  }

  int col = index % cols_;
  int row = index / cols_;

  float tex_w = static_cast<float>(texture_->width());
  float tex_h = static_cast<float>(texture_->height());

  // Top-left origin in texture coordinates (0,0 is bottom-left in OpenGL,
  // but we want index 0 to be top-left of the image).
  // Standard texture coordinates:
  // (0,1) TL ---- (1,1) TR
  //   |             |
  // (0,0) BL ---- (1,0) BR

  float u_start = (col * sprite_width_) / tex_w;
  float u_end = ((col + 1) * sprite_width_) / tex_w;

  // For V, 0 is bottom, 1 is top.
  // Row 0 is at the top of the texture.
  float v_top = (tex_h - (row * sprite_height_)) / tex_h;
  float v_bottom = (tex_h - ((row + 1) * sprite_height_)) / tex_h;

  if (uv_min) {
    *uv_min = {u_start, v_bottom};
  }
  if (uv_max) {
    *uv_max = {u_end, v_top};
  }
}

}  // namespace engine::graphics
