#ifndef PLATFORMER_SCENES_H_
#define PLATFORMER_SCENES_H_

#include <memory>
#include <string>

#include <glm/vec2.hpp>

#include <engine/ecs/registry.h>
#include <engine/scene/scene.h>

#include "components.h"

namespace platformer {

class GameplayScene : public engine::Scene {
 public:
  GameplayScene(const std::string& name, int level);
  void OnAttach() override;
  void OnUpdate(float dt) override;
  void OnRender() override;

 private:
  void LoadLevel(int level);
  void UpdatePlatforms(float dt);
  void UpdatePlayer(float dt);
  void UpdateEnemies(float dt);
  void UpdateCamera();
  void ResetPlayer();

  int level_;
  engine::ecs::EntityID player_entity_ = engine::ecs::kInvalidEntity;
  float camera_x_ = 0.0f;
};

class LevelCompleteScene : public engine::Scene {
 public:
  LevelCompleteScene(const std::string& name, int level);
  void OnAttach() override;
  void OnUpdate(float dt) override;
  void OnRender() override;

 private:
  int level_;
};

}  // namespace platformer

#endif  // PLATFORMER_SCENES_H_
