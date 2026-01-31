#include "graphics/texture.h"

// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

#include <iostream>

#include "graphics/renderer.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace engine::graphics {

Texture* Texture::Create(const std::string& path) {
  std::string full_path = Renderer::Get().ResolveAssetPath(path);
  int width, height, channels;

  // OpenGL expects pixels to start from bottom-left, but most image formats
  // start from top-left. We flip it during load.
  stbi_set_flip_vertically_on_load(true);

  unsigned char* data =
      stbi_load(full_path.c_str(), &width, &height, &channels, 4);

  if (!data) {
    std::cout << "[Texture] Failed to load path: " << path << "\n";
    return nullptr;
  }

  unsigned int id;
  glGenTextures(1, &id);
  glBindTexture(GL_TEXTURE_2D, id);

  // Set wrapping and filtering parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  // GL_LINEAR provides smooth scaling; GL_NEAREST is better for pixel art.
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // Upload the pixel data
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);

  stbi_image_free(data);

  return new Texture(id, width, height, path);
}

Texture::~Texture() { glDeleteTextures(1, &renderer_id_); }

void Texture::Bind(unsigned int slot) const {
  glActiveTexture(GL_TEXTURE0 + slot);
  glBindTexture(GL_TEXTURE_2D, renderer_id_);
}

}  // namespace engine::graphics
