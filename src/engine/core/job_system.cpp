/**
 * @file job_system.cpp
 * @brief JobSystem implementation.
 */

#include <engine/core/job_system.h>

#include <engine/util/logger.h>

namespace engine::core {

void JobSystem::Init() {
  main_thread_id_ = std::this_thread::get_id();
  unsigned int num_threads = std::thread::hardware_concurrency();
  if (num_threads == 0) {
    num_threads = 1;
  }

  LOG_INFO("Initializing JobSystem with %u threads.", num_threads);

  stop_ = false;
  for (unsigned int i = 0; i < num_threads; ++i) {
    workers_.emplace_back(&JobSystem::WorkerLoop, this);
  }
}

void JobSystem::Shutdown() {
  {
    std::lock_guard<std::mutex> lock(queue_mutex_);
    stop_ = true;
  }
  condition_.notify_all();

  for (std::thread& worker : workers_) {
    if (worker.joinable()) {
      worker.join();
    }
  }
  workers_.clear();
  LOG_INFO("JobSystem shut down.");
}

bool JobSystem::IsMainThread() const {
  return std::this_thread::get_id() == main_thread_id_;
}

void JobSystem::WorkerLoop() {
  while (true) {
    std::function<void()> task;
    {
      std::unique_lock<std::mutex> lock(queue_mutex_);
      condition_.wait(lock, [this]() { return stop_ || !tasks_.empty(); });

      if (stop_ && tasks_.empty()) {
        return;
      }

      task = std::move(tasks_.front());
      tasks_.pop();
    }
    task();
  }
}

}  // namespace engine::core
