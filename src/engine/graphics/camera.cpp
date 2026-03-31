/**
 * @file camera.cpp
 * @brief Camera class implementation.
 */

#include <glm/gtc/matrix_transform.hpp>

#include <engine/graphics/camera.h>

namespace engine::graphics {

Camera::Camera(float left, float right, float bottom, float top)
    : position_(0.0f, 0.0f, 0.0f) {
  projection_matrix_ = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
  view_matrix_ = glm::mat4(1.0f);
  view_projection_matrix_ = projection_matrix_ * view_matrix_;
}

void Camera::SetProjection(float left, float right, float bottom, float top) {
  projection_matrix_ = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
  UpdateMatrices();
}

void Camera::SetPosition(const glm::vec3& position) {
  position_ = position;
  UpdateMatrices();
}

void Camera::UpdateMatrices() {
  glm::mat4 transform =
      glm::translate(glm::mat4(1.0f), position_) *
      glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0, 0, 1));

  view_matrix_ = glm::inverse(transform);
  view_projection_matrix_ = projection_matrix_ * view_matrix_;
}

}  // namespace engine::graphics
