/**
 * @file tween_manager.cpp
 * @brief TweenManager implementation.
 */

#include <engine/util/tween_manager.h>

namespace engine::util {

TweenManager& TweenManager::Get() {
  static TweenManager instance;
  return instance;
}

void TweenManager::Update(float delta_time) {
  // Add pending tasks
  if (!pending_tasks_.empty()) {
    for (auto& task : pending_tasks_) {
      tasks_.push_back(std::move(task));
    }
    pending_tasks_.clear();
  }

  // Update active tasks
  for (auto it = tasks_.begin(); it != tasks_.end();) {
    if ((*it)->Update(delta_time)) {
      it = tasks_.erase(it);
    } else {
      ++it;
    }
  }
}

void TweenManager::StopTween(uint32_t id) {
  for (auto& task : tasks_) {
    if (task->id() == id) {
      task->Stop();
      return;
    }
  }
  for (auto& task : pending_tasks_) {
    if (task->id() == id) {
      task->Stop();
      return;
    }
  }
}

void TweenManager::Clear() {
  tasks_.clear();
  pending_tasks_.clear();
}

}  // namespace engine::util
