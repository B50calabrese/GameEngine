#ifndef BREAKOUT_COMPONENTS_H_
#define BREAKOUT_COMPONENTS_H_

#include <glm/glm.hpp>

namespace breakout {

struct PaddleComponent {
  float speed = 500.0f;
  glm::vec2 base_size = {120.0f, 20.0f};
};

struct BallComponent {
  float radius = 10.0f;
};

struct BrickComponent {
  bool is_destroyed = false;
};

}  // namespace breakout

#endif  // BREAKOUT_COMPONENTS_H_
