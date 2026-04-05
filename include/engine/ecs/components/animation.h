/**
 * @file animation.h
 * @brief Component for sprite animations.
 */

#ifndef INCLUDE_ENGINE_ECS_COMPONENTS_ANIMATION_H_
#define INCLUDE_ENGINE_ECS_COMPONENTS_ANIMATION_H_

#include <string>
#include <vector>

namespace engine::ecs::components {

/** @brief Represents a single animation frame. */
struct AnimationFrame {
  int sprite_index = 0;
  float duration = 0.1f;  // Seconds
};

/** @brief Represents an animation clip. */
struct AnimationClip {
  std::string name;
  std::vector<AnimationFrame> frames;
  bool loop = true;
};

/** @brief Component that manages sprite animations. */
struct Animation {
  std::vector<AnimationClip> clips;
  std::string current_clip = "";
  int current_frame = 0;
  float timer = 0.0f;
  bool is_playing = true;

  /** @brief Helper to find a clip by name. */
  const AnimationClip* GetClip(const std::string& name) const {
    for (const auto& clip : clips) {
      if (clip.name == name) {
        return &clip;
      }
    }
    return nullptr;
  }
};

}  // namespace engine::ecs::components

#endif  // INCLUDE_ENGINE_ECS_COMPONENTS_ANIMATION_H_
