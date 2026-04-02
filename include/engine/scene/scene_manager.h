/**
 * @file scene_manager.h
 * @brief Manages scene transitions and the scene stack.
 */

#ifndef INCLUDE_ENGINE_SCENE_SCENE_MANAGER_H_
#define INCLUDE_ENGINE_SCENE_SCENE_MANAGER_H_

#include <memory>
#include <string>
#include <vector>

#include <engine/scene/scene.h>

namespace engine {

class Application;

/**
 * @brief Manages the lifecycle and stack of game scenes.
 */
class SceneManager {
 public:
  static SceneManager& Get() {
    static SceneManager instance;
    return instance;
  }

  void SetScene(std::unique_ptr<Scene> new_scene);
  void PushScene(std::unique_ptr<Scene> overlay);
  void PopScene();

  Scene* GetActiveScene();

 private:
  friend class Application;

  void UpdateActiveScene(float delta_time);
  void RenderActiveScene();
  bool DispatchInput();

 private:
  SceneManager() = default;
  ~SceneManager() = default;

  std::vector<std::unique_ptr<Scene>> scene_stack_;
};
}  // namespace engine

#endif  // INCLUDE_ENGINE_SCENE_SCENE_MANAGER_H_
