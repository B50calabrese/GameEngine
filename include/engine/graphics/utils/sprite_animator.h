/**
 * @file sprite_animator.h
 * @brief Sprite animation management and playback.
 */

#ifndef INCLUDE_ENGINE_GRAPHICS_UTILS_SPRITE_ANIMATOR_H_
#define INCLUDE_ENGINE_GRAPHICS_UTILS_SPRITE_ANIMATOR_H_

#include <string>
#include <unordered_map>
#include <vector>

namespace engine::graphics::utils {

/**
 * @brief Defines a sequence of sprite indices and playback properties.
 */
struct AnimationClip {
  std::vector<int> frames;
  float fps = 10.0f;
  bool loop = true;
};

/**
 * @brief Singleton manager for storing and retrieving AnimationClips.
 */
class AnimationManager {
 public:
  /** @brief Gets the singleton instance. */
  static AnimationManager& Get();

  /**
   * @brief Registers an animation clip with a name.
   * @param name The name of the animation.
   * @param clip The animation clip definition.
   */
  void AddClip(const std::string& name, const AnimationClip& clip);

  /**
   * @brief Retrieves an animation clip by name.
   * @param name The name of the animation.
   * @return Pointer to the clip, or nullptr if not found.
   */
  [[nodiscard]] const AnimationClip* GetClip(const std::string& name) const;

  /** @brief Removes all registered clips. */
  void Clear();

 private:
  AnimationManager() = default;
  ~AnimationManager() = default;

  std::unordered_map<std::string, AnimationClip> clips_;
};

/**
 * @brief Handles playback state for sprite animations.
 */
class SpriteAnimator {
 public:
  SpriteAnimator() = default;

  /**
   * @brief Starts playing an animation clip by name.
   * @param name The name of the clip to play.
   */
  void Play(const std::string& name);

  /**
   * @brief Updates the animation timer.
   * @param delta_time The time elapsed since the last update, in seconds.
   */
  void Update(float delta_time);

  /**
   * @brief Gets the current frame index from the active clip.
   * @return The sprite index, or 0 if no clip is playing.
   */
  [[nodiscard]] int GetCurrentFrame() const;

  /** @brief Returns true if an animation is currently playing and not finished.
   */
  [[nodiscard]] bool IsPlaying() const { return playing_; }

  /** @brief Returns true if a non-looping animation has reached its end. */
  [[nodiscard]] bool IsFinished() const { return finished_; }

  /** @brief Gets the name of the currently playing clip. */
  [[nodiscard]] const std::string& current_clip_name() const {
    return current_clip_name_;
  }

 private:
  const AnimationClip* current_clip_ = nullptr;
  std::string current_clip_name_;
  float elapsed_time_ = 0.0f;
  bool playing_ = false;
  bool finished_ = false;
};

}  // namespace engine::graphics::utils

#endif  // INCLUDE_ENGINE_GRAPHICS_UTILS_SPRITE_ANIMATOR_H_
