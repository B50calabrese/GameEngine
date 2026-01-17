#ifndef SCENE_H
#define SCENE_H

#include <string>

namespace engine {

/**
 * Abstract base class for all scenes in a game. The client inherits from this
 * to create specific screens (Menu, Levels, etc).
 */
class Scene {
 public:
  Scene(const std::string& name) : debug_name(name) {}
  virtual ~Scene() = default;

  /** Called when the scene is added to the manager (e.g., loading assets) */
  virtual void OnAttach() {}

  /**
   * Called when the scene is removed from the manager (e.g., clearing assets)
   */
  virtual void OnDetach() {}

  /** Called every frame for logic updates */
  virtual void OnUpdate(float deltaTimeSeconds) {}

  /** Called every frame for drawing calls */
  virtual void OnRender() {}

  /**
   * Called by the application when an input event occurs. Returning 'true' can
   * signal the Engine that the event was "handled" and shouldn't propagate.
   */
  virtual bool OnInput() { return false; }

  inline const std::string& GetName() const { return debug_name; }

 protected:
  std::string debug_name;
};
}  // namespace engine

#endif  // SCENE_H