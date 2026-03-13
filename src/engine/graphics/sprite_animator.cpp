/**
 * @file sprite_animator.cpp
 * @brief Sprite animation management and playback implementation.
 */

#include <engine/graphics/sprite_animator.h>
#include <engine/util/logger.h>

#include <algorithm>
#include <cmath>

namespace engine::graphics {

// --- AnimationManager Implementation ---

AnimationManager& AnimationManager::Get() {
  static AnimationManager instance;
  return instance;
}

void AnimationManager::AddClip(const std::string& name,
                               const AnimationClip& clip) {
  clips_[name] = clip;
}

const AnimationClip* AnimationManager::GetClip(const std::string& name) const {
  auto it = clips_.find(name);
  if (it != clips_.end()) {
    return &it->second;
  }
  return nullptr;
}

void AnimationManager::Clear() { clips_.clear(); }

// --- SpriteAnimator Implementation ---

void SpriteAnimator::Play(const std::string& name) {
  const AnimationClip* clip = AnimationManager::Get().GetClip(name);
  if (!clip) {
    LOG_ERR("Failed to play animation '%s': Clip not found.", name.c_str());
    playing_ = false;
    current_clip_ = nullptr;
    current_clip_name_ = "";
    return;
  }

  current_clip_ = clip;
  current_clip_name_ = name;
  elapsed_time_ = 0.0f;
  playing_ = true;
  finished_ = false;
}

void SpriteAnimator::Update(float delta_time) {
  if (!playing_ || !current_clip_ || current_clip_->frames.empty()) {
    return;
  }

  elapsed_time_ += delta_time;

  float duration = static_cast<float>(current_clip_->frames.size()) / current_clip_->fps;

  if (elapsed_time_ >= duration) {
    if (current_clip_->loop) {
      // Wrap around
      elapsed_time_ = std::fmod(elapsed_time_, duration);
    } else {
      // Stay on last frame
      elapsed_time_ = duration;
      playing_ = false;
      finished_ = true;
    }
  }
}

int SpriteAnimator::GetCurrentFrame() const {
  if (!current_clip_ || current_clip_->frames.empty()) {
    return 0;
  }

  // Calculate index based on elapsed time and FPS
  int index = static_cast<int>(elapsed_time_ * current_clip_->fps);

  // Clamp index to avoid overflow due to floating point precision at the very end of non-looping animations
  int max_index = static_cast<int>(current_clip_->frames.size()) - 1;
  index = std::clamp(index, 0, max_index);

  return current_clip_->frames[index];
}

}  // namespace engine::graphics
