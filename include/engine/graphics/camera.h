/**
 * @file camera.h
 * @brief Orthographic camera for 2D rendering.
 */

#ifndef INCLUDE_ENGINE_GRAPHICS_CAMERA_H_
#define INCLUDE_ENGINE_GRAPHICS_CAMERA_H_

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

namespace engine::graphics {

/**
 * @brief Simple 2D Camera that manages the Orthographic projection.
 */
class Camera {
 public:
  /** @brief Constructs a camera. */
  Camera(float left, float right, float bottom, float top);

  /** @brief Updates the boundaries. */
  void SetProjection(float left, float right, float bottom, float top);

  /** @brief Moves the camera. */
  void SetPosition(const glm::vec3& position);

  /** @brief Gets the position. */
  const glm::vec3& GetPosition() const { return position_; }

  /** @brief Gets the view-projection matrix. */
  const glm::mat4& GetViewProjectionMatrix() const {
    return GetViewProjectionMatrix_;
  }

 private:
  void UpdateMatrices();

  glm::mat4 projection_matrix_;
  glm::mat4 view_matrix_;
  glm::mat4 GetViewProjectionMatrix_;
  glm::vec3 position_;
};

}  // namespace engine::graphics

#endif  // INCLUDE_ENGINE_GRAPHICS_CAMERA_H_
