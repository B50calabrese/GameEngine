#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>

namespace engine::graphics {

/**
 * High-level wrapper for OpenGL Texture resources. Manages loading, GPU upload,
 * and automatic memory cleanup.
 */
class Texture {
 public:
  /**
   * @brief Loads an image from disk and uploads it to the GPU.
   * @param path File path to the image (PNG, JPG, etc.)
   * @return A pointer to the Texture object, or nullptr if loading fails.
   */
  static Texture* Create(const std::string& path);

  ~Texture();

  void Bind(unsigned int slot = 0) const;

  inline unsigned int GetID() const { return renderer_id; }

  inline int GetWidth() const { return width; }

  inline int GetHeight() const { return height; }

 private:
  Texture(unsigned int id, int w, int h, const std::string& p)
      : renderer_id(id), width(w), height(h), path(p) {}

  unsigned int renderer_id;
  int width, height;
  std::string path;
};

}  // namespace engine::graphics

#endif  // TEXTURE_H