#ifndef DEMOS_GAMES_PLATFORMER_SCENES_H_
#define DEMOS_GAMES_PLATFORMER_SCENES_H_

#include <memory>
#include <string>
#include <unordered_map>

#include <glm/vec2.hpp>

#include <engine/ecs/registry.h>
#include <engine/graphics/texture.h>
#include <engine/scene/scene.h>

#include "components.h"

namespace platformer {

class MenuScene : public engine::Scene {
 public:
  MenuScene(const std::string& name);
  void OnAttach() override;
  void OnUpdate(float dt) override;
  void OnRender() override;

 private:
  bool show_controls_ = false;
};

class GameplayScene : public engine::Scene {
 public:
  GameplayScene(const std::string& name, int level);
  void OnAttach() override;
  void OnUpdate(float dt) override;
  void OnRender() override;

 private:
  void LoadLevel(int level);
  void CreatePlatform(glm::vec2 pos, glm::vec2 size, PlatformType type,
                      glm::vec2 start = {0, 0}, glm::vec2 end = {0, 0});
  void CreateEnemy(glm::vec2 pos, bool patrolling, glm::vec2 start = {0, 0},
                   glm::vec2 end = {0, 0});
  void CreateGoal(glm::vec2 pos);
  void UpdatePlatforms(float dt);
  void UpdatePlayer(float dt);
  void UpdateEnemies(float dt);
  void UpdateCamera();
  void ResetPlayer();

  int level_;
  engine::ecs::EntityID player_entity_;
  float camera_x_ = 0.0f;
  std::shared_ptr<engine::graphics::Texture> bg_tex_;
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

#endif  // DEMOS_GAMES_PLATFORMER_SCENES_H_
