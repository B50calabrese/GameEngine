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
 * @brief Abstract base class for game scenes.
 */
class Scene {
 public:
  /** @brief Constructs a new Scene. */
  explicit Scene(std::string name) : name_(std::move(name)) {}

  /** @brief Virtual destructor. */
  virtual ~Scene() = default;

  /** @brief Called when the scene is attached. */
  virtual void OnAttach() {}

  /** @brief Called when the scene is detached. */
  virtual void OnDetach() {}

  /** @brief Called for logic updates. */
  virtual void OnUpdate(float delta_time) {}

  /** @brief Called for rendering. */
  virtual void OnRender() {}

  /** @brief Called for input handling. */
  virtual bool OnInput() { return false; }

  /** @brief Gets the debug name. */
  const std::string& GetName() const { return name_; }

  /** @brief Gets the ECS registry. */
  ecs::Registry& GetRegistry() { return registry_; }

 protected:
  std::string name_;
  ecs::Registry registry_;
};

}  // namespace engine

#endif  // INCLUDE_ENGINE_SCENE_SCENE_H_
