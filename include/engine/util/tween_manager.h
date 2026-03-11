/**
 * @file tween_manager.h
 * @brief Utility for managing and updating tweens.
 */

#ifndef INCLUDE_ENGINE_UTIL_TWEEN_MANAGER_H_
#define INCLUDE_ENGINE_UTIL_TWEEN_MANAGER_H_

#include <engine/util/easing.h>

#include <algorithm>
#include <functional>
#include <memory>
#include <vector>

#include <glm/glm.hpp>

namespace engine::util {

/**
 * @brief Interface for a generic tween task.
 */
class ITweenTask {
 public:
  virtual ~ITweenTask() = default;
  virtual bool Update(float delta_time) = 0;
  virtual uint32_t id() const = 0;
  virtual void Stop() = 0;
};

template <typename T>
class TweenBuilder;

/**
 * @brief Manages active tween tasks and provides a fluent API for creating them.
 */
class TweenManager {
 public:
  /**
   * @brief Gets the singleton instance of the TweenManager.
   */
  static TweenManager& Get();

  /**
   * @brief Updates all active tweens.
   * @param delta_time The time elapsed since the last update, in seconds.
   */
  void Update(float delta_time);

  /**
   * @brief Starts a new tween.
   * @tparam T The type of value to tween.
   * @param start The starting value.
   * @param end The ending value.
   * @param duration The duration of the tween in seconds.
   * @return A TweenBuilder to configure and play the tween.
   */
  template <typename T>
  TweenBuilder<T> Tween(T start, T end, float duration);

  /**
   * @brief Stops and removes a tween by its ID.
   * @param id The ID of the tween to stop.
   */
  void StopTween(uint32_t id);

  /**
   * @brief Stops and removes all active tweens.
   */
  void Clear();

  /**
   * @brief Internal use only. Gets the next available tween ID.
   */
  uint32_t GetNextId() { return next_id_++; }

  /**
   * @brief Internal use only. Adds a task to the manager.
   */
  void AddTask(std::unique_ptr<ITweenTask> task) {
    pending_tasks_.push_back(std::move(task));
  }

 private:
  TweenManager() = default;
  ~TweenManager() = default;

  std::vector<std::unique_ptr<ITweenTask>> tasks_;
  std::vector<std::unique_ptr<ITweenTask>> pending_tasks_;
  uint32_t next_id_ = 1;
};

/**
 * @brief Fluent API builder for creating and configuring tweens.
 */
template <typename T>
class TweenBuilder {
 public:
  TweenBuilder(T start, T end, float duration)
      : start_(start), end_(end), duration_(duration) {}

  TweenBuilder& Ease(EaseType type) {
    ease_type_ = type;
    return *this;
  }

  TweenBuilder& Delay(float seconds) {
    delay_ = seconds;
    return *this;
  }

  TweenBuilder& Loop(bool loop = true) {
    loop_ = loop;
    return *this;
  }

  TweenBuilder& PingPong(bool ping_pong = true) {
    ping_pong_ = ping_pong;
    return *this;
  }

  TweenBuilder& OnUpdate(std::function<void(const T&, float)> callback) {
    on_update_ = callback;
    return *this;
  }

  TweenBuilder& OnComplete(std::function<void()> callback) {
    on_complete_ = callback;
    return *this;
  }

  /**
   * @brief Registers the configured tween with the TweenManager and starts it.
   * @return The ID of the created tween.
   */
  uint32_t Play();

 private:
  T start_;
  T end_;
  float duration_;
  float delay_ = 0.0f;
  EaseType ease_type_ = EaseType::Linear;
  bool loop_ = false;
  bool ping_pong_ = false;
  std::function<void(const T&, float)> on_update_;
  std::function<void()> on_complete_;
};

/**
 * @brief Implementation of a specific tween task.
 */
template <typename T>
class TweenTask : public ITweenTask {
 public:
  TweenTask(uint32_t id, T start, T end, float duration, float delay,
            EaseType ease_type, bool loop, bool ping_pong,
            std::function<void(const T&, float)> on_update,
            std::function<void()> on_complete)
      : id_(id),
        start_(start),
        end_(end),
        duration_(duration),
        delay_(delay),
        ease_type_(ease_type),
        loop_(loop),
        ping_pong_(ping_pong),
        on_update_(on_update),
        on_complete_(on_complete) {}

  bool Update(float delta_time) override {
    if (stopped_) return true;

    if (delay_ > 0.0f) {
      delay_ -= delta_time;
      return false;
    }

    elapsed_time_ += delta_time;
    float progress = std::clamp(elapsed_time_ / duration_, 0.0f, 1.0f);
    float eased_progress = Easing::Apply(ease_type_, progress);

    T current_value = Interpolate(start_, end_, eased_progress);

    if (on_update_) {
      on_update_(current_value, progress);
    }

    if (elapsed_time_ >= duration_) {
      if (ping_pong_) {
        std::swap(start_, end_);
        elapsed_time_ = 0.0f;
        return false;
      } else if (loop_) {
        elapsed_time_ = 0.0f;
        return false;
      } else {
        if (on_complete_) {
          on_complete_();
        }
        return true;  // Task finished
      }
    }

    return false;
  }

  uint32_t id() const override { return id_; }
  void Stop() override { stopped_ = true; }

 private:
  T Interpolate(const T& start, const T& end, float t) {
    return start + (end - start) * t;
  }

  uint32_t id_;
  T start_;
  T end_;
  float duration_;
  float delay_;
  EaseType ease_type_;
  bool loop_;
  bool ping_pong_;
  std::function<void(const T&, float)> on_update_;
  std::function<void()> on_complete_;

  float elapsed_time_ = 0.0f;
  bool stopped_ = false;
};

// Template implementations

template <typename T>
TweenBuilder<T> TweenManager::Tween(T start, T end, float duration) {
  return TweenBuilder<T>(start, end, duration);
}

template <typename T>
uint32_t TweenBuilder<T>::Play() {
  uint32_t id = TweenManager::Get().GetNextId();
  auto task = std::make_unique<TweenTask<T>>(
      id, start_, end_, duration_, delay_, ease_type_, loop_, ping_pong_,
      on_update_, on_complete_);
  TweenManager::Get().AddTask(std::move(task));
  return id;
}

}  // namespace engine::util

#endif  // INCLUDE_ENGINE_UTIL_TWEEN_MANAGER_H_
