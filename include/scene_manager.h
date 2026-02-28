#ifndef INCLUDE_SCENE_MANAGER_H_
#define INCLUDE_SCENE_MANAGER_H_

#include <memory>
#include <string>
#include <vector>

#include "scene.h"

namespace engine {

// Forward declaration of the Application.
class Application;

/**
 * @brief Manages the lifecycle and stack of game scenes.
 */
class SceneManager {
 public:
  /**
   * @brief Returns a reference to the singleton `SceneManager` instance.
   * @return Reference to the SceneManager.
   */
  static SceneManager& get() {
    static SceneManager instance;
    return instance;
  }

  /**
   * @brief Clears the stack and sets a single active scene.
   * @param new_scene Unique pointer to the new scene.
   */
  void set_scene(std::unique_ptr<Scene> new_scene);

  /**
   * @brief Pushes an overlay scene (like a Pause Menu) onto the stack.
   * @param overlay Unique pointer to the overlay scene.
   */
  void push_scene(std::unique_ptr<Scene> overlay);

  /**
   * @brief Pops/removes the top scene on the stack (like removing the Pause
   * Menu).
   */
  void pop_scene();

 private:
  // The Application needs to drive the scenes, but we don't want
  // to expose these dispatchers to the end-user/client.
  friend class Application;

  /**
   * @brief Updates the active scene.
   * @param delta_time Time elapsed since last frame.
   */
  void update_active_scene(float delta_time);

  /**
   * @brief Renders the active scene(s).
   */
  void render_active_scene();

  /**
   * @brief Dispatches input to the top scene.
   * @return `true` if the scene handled/consumed the input.
   */
  bool dispatch_input();

 private:
  SceneManager() = default;
  ~SceneManager() = default;

  std::vector<std::unique_ptr<Scene>> scene_stack_;
};
}  // namespace engine

#endif  // INCLUDE_SCENE_MANAGER_H_
