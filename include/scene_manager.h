#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include <memory>
#include <string>
#include <vector>

#include "scene.h"

namespace engine {

// Forward declaration of the Application.
class Application;

/**
 * Manages the lifecycle and stack of game scenes.
 */
class SceneManager {
 public:
  static SceneManager& Get() {
    static SceneManager instance;
    return instance;
  }

  /**
   * Clears the stack and sets a single active scene.
   */
  void SetScene(std::unique_ptr<Scene> newScene);

  /**
   * Pushes an overlay scene (like a Pause Menu) onto the stack.
   */
  void PushScene(std::unique_ptr<Scene> overlay);

  /**
   * Pops/removes the top scene on the stack (like removing the Pause Menu).
   */
  void PopScene();

 private:
  // The Application needs to drive the scenes, but we don't want
  // to expose these dispatchers to the end-user/client.
  friend class Application;

  void UpdateActiveScene(float delta_time);
  void RenderActiveScene();

  /**
   * Dispatches input to the top scene.
   * @return true if the scene handled/consumed the input.
   */
  bool DispatchInput();

 private:
  SceneManager() = default;
  ~SceneManager() = default;

  std::vector<std::unique_ptr<Scene>> scene_stack;
};
}  // namespace engine

#endif  // SCENE_MANAGER_H