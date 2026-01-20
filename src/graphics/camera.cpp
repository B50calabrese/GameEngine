#include "graphics/camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

namespace engine::graphics {

Camera::Camera(float left, float right, float bottom, float top)
    : projection_matrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)),
      view_matrix(1.0f),
      position(0.0f, 0.0f, 0.0f) {
  UpdateMatrices();
}

void Camera::SetProjection(float left, float right, float bottom, float top) {
  projection_matrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
  UpdateMatrices();
}

void Camera::SetPosition(const glm::vec3& position) {
  this->position = position;
  UpdateMatrices();
}

// Private functions

void Camera::UpdateMatrices() {
  // In 2D, "moving" the camera is just the inverse translation of the world.
  // We use -position because if the camera moves Right, the world moves
  // Left.
  view_matrix = glm::translate(glm::mat4(1.0f), -position);

  // Order matters: Projection * View * Model (Model is handled per-object)
  view_projection_matrix = projection_matrix * view_matrix;
}
}  // namespace engine::graphics