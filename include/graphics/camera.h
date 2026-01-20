#ifndef CAMERA_H
#define CAMERA_H

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

namespace engine::graphics {

/**
 * Simple 2D Camera that manages the Orthographic projection.
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
   * Create a camera by defining the view boundaries.
   * Note: Flipping 'bottom' and 'top' determines if (0,0) is Top-Left or
   * Bottom-Left.
   */
  Camera(float left, float right, float bottom, float top);

  /**
   * Updates the boundaries (useful for window resizing).
   */
  void SetProjection(float left, float right, float bottom, float top);

  /**
   * Moves the camera in world space.
   */
  void SetPosition(const glm::vec3& position);

  const glm::vec3& GetPosition() const { return position; }
  const glm::mat4& GetViewProjectionMatrix() const {
    return view_projection_matrix;
  }

 private:
  /**
   * Re-calculates the final matrix whenever position or projection
   * changes.
   */
  void UpdateMatrices();

 private:
  glm::mat4 projection_matrix;       // The "Lens" (Resolution/Aspect)
  glm::mat4 view_matrix;             // The "Eyeball" (Position/Rotation)
  glm::mat4 view_projection_matrix;  // The cached result sent to Shaders

  glm::vec3 position;  // Camera's location in world units
};

}  // namespace engine::graphics

#endif  // CAMERA_H