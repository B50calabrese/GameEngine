#ifndef BREAKOUT_GAMEPLAY_SCENE_H_
#define BREAKOUT_GAMEPLAY_SCENE_H_

#include <vector>

#include <engine/scene/scene.h>

#include "components.h"

namespace breakout {

class GameplayScene : public engine::Scene {
 public:
  explicit GameplayScene(const std::string& name);

  void OnAttach() override;
  void OnUpdate(float dt) override;
  void OnRender() override;

 private:
  void ResetGame();
  void UpdatePaddle(float dt);
  void UpdateBall(float dt);
  void EmitParticles(glm::vec2 pos, glm::vec4 color);

  struct BrickInfo {
    engine::ecs::EntityID id;
    glm::vec2 pos;
    glm::vec2 size;
  };

  engine::ecs::EntityID paddle_ = engine::ecs::kInvalidEntity;
  engine::ecs::EntityID ball_ = engine::ecs::kInvalidEntity;
  engine::ecs::EntityID emitter_entity_ = engine::ecs::kInvalidEntity;
  engine::ecs::EntityID score_label_ = engine::ecs::kInvalidEntity;
  std::vector<BrickInfo> bricks_;
  int bricks_hit_ = 0;
  bool is_game_over_ = false;
  float shake_time_ = 0.0f;
};

}  // namespace breakout

#endif  // BREAKOUT_GAMEPLAY_SCENE_H_
