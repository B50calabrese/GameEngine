/**
 * @file performance_overlay.h
 * @brief Overlay for displaying game health and performance metrics.
 */

#ifndef INCLUDE_ENGINE_UTIL_PERFORMANCE_OVERLAY_H_
#define INCLUDE_ENGINE_UTIL_PERFORMANCE_OVERLAY_H_

#include <string>
#include <vector>

#include <engine/input/input_manager.h>

namespace engine::util {

/**
 * @brief Manages a performance overlay for in-game metrics.
 */
class PerformanceOverlay {
 public:
  static PerformanceOverlay& Get();

  /** @brief Toggles the overlay's visibility. */
  void Toggle() { visible_ = !visible_; }

  /** @brief Returns true if the overlay is visible. */
  bool is_visible() const { return visible_; }

  /** @brief Sets the key used to toggle the overlay. */
  void SetToggleKey(KeyCode key) { toggle_key_ = key; }

  /** @brief Gets the key used to toggle the overlay. */
  KeyCode GetToggleKey() const { return toggle_key_; }

  /** @brief Updates performance metrics. */
  void Update(double delta_time);

  /** @brief Renders the overlay. */
  void Render();

 private:
  PerformanceOverlay() = default;
  ~PerformanceOverlay() = default;

  bool visible_ = false;
  KeyCode toggle_key_ = KeyCode::kF1;

  double frame_time_accum_ = 0.0;
  int frame_count_ = 0;
  float current_fps_ = 0.0f;
  float current_frame_time_ms_ = 0.0f;

  float cpu_usage_percent_ = 0.0f;
  long long last_proc_time_ = 0;
  long long last_sys_time_ = 0;

  double ram_usage_mb_ = 0.0;

  void UpdateSystemMetrics();
};

}  // namespace engine::util

#endif  // INCLUDE_ENGINE_UTIL_PERFORMANCE_OVERLAY_H_
