#ifndef INCLUDE_GRAPHICS_TEXTURE_H_
#define INCLUDE_GRAPHICS_TEXTURE_H_

#include <memory>
#include <string>

namespace engine::graphics {

/**
 * @brief High-level wrapper for OpenGL Texture resources.
 *
 * Manages loading, GPU upload, and automatic memory cleanup.
 */
class Texture {
 public:
  /**
   * @brief Loads an image from disk and uploads it to the GPU.
   * @param path File path to the image (PNG, JPG, etc.)
   * @return A unique pointer to the Texture object, or nullptr if loading fails.
   */
  static std::unique_ptr<Texture> Create(const std::string& path);

  /** @brief Destructor deletes the OpenGL texture. */
  ~Texture();

  /**
   * @brief Binds the texture to a specific texture slot.
   * @param slot The texture slot to bind to (default is 0).
   */
  void Bind(unsigned int slot = 0) const;

  /**
   * @brief Gets the OpenGL ID of the texture.
   * @return The renderer ID.
   */
  inline unsigned int id() const { return renderer_id_; }

  /**
   * @brief Gets the width of the texture.
   * @return Width in pixels.
   */
  inline int width() const { return width_; }

  /**
   * @brief Gets the height of the texture.
   * @return Height in pixels.
   */
  inline int height() const { return height_; }

 private:
  Texture(unsigned int id, int w, int h, const std::string& p)
      : renderer_id_(id), width_(w), height_(h), path_(p) {}

  unsigned int renderer_id_;
  int width_;
  int height_;
  std::string path_;
};

}  // namespace engine::graphics

#endif  // INCLUDE_GRAPHICS_TEXTURE_H_
