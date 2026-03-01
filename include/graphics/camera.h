#ifndef INCLUDE_GRAPHICS_CAMERA_H_
#define INCLUDE_GRAPHICS_CAMERA_H_

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

namespace engine::graphics {

/**
 * @brief Simple 2D Camera that manages the Orthographic projection.
 *
 * It manages three matrices internally:
 * 1. Projection Matrix: Maps world coordinates (e.g., 0 to 1920) to GPU space
 * (-1 to 1). This defines your "Resolution" or "Aspect Ratio".
 * 2. View Matrix: Represents the Camera's position and rotation in the world.
 * Moving the camera Right (+X) actually moves the View Matrix Left (-X).
 * 3. ViewProjection Matrix: The combination (Proj * View). This is what we
 * send to the Shader to transform every vertex in one go.
 */
class Camera {
 public:
  /**
   * @brief Create a camera by defining the view boundaries.
   *
   * Note: Flipping 'bottom' and 'top' determines if (0,0) is Top-Left or
   * Bottom-Left.
   *
   * @param left Left boundary.
   * @param right Right boundary.
   * @param bottom Bottom boundary.
   * @param top Top boundary.
   */
  Camera(float left, float right, float bottom, float top);

  /**
   * @brief Updates the boundaries (useful for window resizing).
   *
   * @param left Left boundary.
   * @param right Right boundary.
   * @param bottom Bottom boundary.
   * @param top Top boundary.
   */
  void set_projection(float left, float right, float bottom, float top);

  /**
   * @brief Moves the camera in world space.
   * @param position The new position.
   */
  void set_position(const glm::vec3& position);

  /**
   * @brief Gets the current position of the camera.
   * @return The position.
   */
  const glm::vec3& position() const { return position_; }

  /**
   * @brief Gets the cached view-projection matrix.
   * @return The matrix.
   */
  const glm::mat4& view_projection_matrix() const {
    return view_projection_matrix_;
  }

 private:
  /**
   * @brief Re-calculates the final matrix whenever position or projection
   * changes.
   */
  void UpdateMatrices();

  glm::mat4 projection_matrix_;       // The "Lens" (Resolution/Aspect)
  glm::mat4 view_matrix_;             // The "Eyeball" (Position/Rotation)
  glm::mat4 view_projection_matrix_;  // The cached result sent to Shaders

  glm::vec3 position_;  // Camera's location in world units
};

}  // namespace engine::graphics

#endif  // INCLUDE_GRAPHICS_CAMERA_H_
