/**
 * @file scene.h
 * @brief Defines the base Scene class.
 */

#ifndef INCLUDE_ENGINE_SCENE_SCENE_H_
#define INCLUDE_ENGINE_SCENE_SCENE_H_

#include <string>

#include <engine/ecs/registry.h>

namespace engine {

/**
 * @brief Abstract base class for all scenes in a game.
 *
 * The client inherits from this to create specific screens (Menu, Levels, etc).
 */
class Scene {
 public:
  /**
   * @brief Constructs a new Scene.
   * @param name The debug name of the scene.
   */
  Scene(const std::string& name) : debug_name_(name) {}

  /** @brief Virtual destructor. */
  virtual ~Scene() = default;

  /**
   * @brief Called when the scene is added to the manager (e.g., loading
   * assets).
   */
  virtual void OnAttach() {}

  /**
   * @brief Called when the scene is removed from the manager (e.g., clearing
   * assets).
   */
  virtual void OnDetach() {}

  /**
   * @brief Called every frame for logic updates.
   * @param delta_time_seconds The time elapsed since the last frame, in
   * seconds.
   */
  virtual void OnUpdate(float delta_time_seconds) {}

  /** @brief Called every frame for drawing calls. */
  virtual void OnRender() {}

  /**
   * @brief Called by the application when an input event occurs.
   * @return `true` if the event was "handled" and shouldn't propagate.
   */
  virtual bool OnInput() { return false; }

  /**
   * @brief Gets the debug name of the scene.
   * @return The debug name.
   */
  inline const std::string& name() const { return debug_name_; }

  /**
   * @brief Returns the registry associated with this scene.
   */
  ecs::Registry& registry() { return registry_; }

 protected:
  /** @brief The debug name of the scene. */
  std::string debug_name_;
  /** @brief The ECS registry for this scene. */
  ecs::Registry registry_;
};
}  // namespace engine

#endif  // INCLUDE_ENGINE_SCENE_SCENE_H_
