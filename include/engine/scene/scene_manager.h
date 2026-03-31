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
  /** @brief Gets the singleton instance. */
  static SceneManager& Get() {
    static SceneManager instance;
    return instance;
  }

  /** @brief Sets the current scene, clearing the stack. */
  void SetScene(std::unique_ptr<Scene> new_scene);

  /** @brief Pushes a new scene onto the stack. */
  void PushScene(std::unique_ptr<Scene> overlay);

  /** @brief Pops the top scene from the stack. */
  void PopScene();

  /** @brief Gets the active (top) scene. */
  Scene* GetActiveScene();

  /** @brief Updates the active scene. */
  void UpdateActiveScene(float delta_time);

  /** @brief Renders the entire scene stack. */
  void RenderActiveScene();

  /** @brief Dispatches input to the active scene. */
  bool DispatchInput();

 private:
  SceneManager() = default;
  ~SceneManager() = default;

  std::vector<std::unique_ptr<Scene>> scene_stack_;
};

}  // namespace engine

#endif  // INCLUDE_ENGINE_SCENE_SCENE_MANAGER_H_
