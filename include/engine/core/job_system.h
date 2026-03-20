/**
 * @file job_system.h
 * @brief Utility for spawning work in threads.
 */

#ifndef INCLUDE_ENGINE_CORE_JOB_SYSTEM_H_
#define INCLUDE_ENGINE_CORE_JOB_SYSTEM_H_

#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

#include <engine/util/logger.h>

namespace engine::core {

/**
 * @brief Singleton class that manages a pool of worker threads for parallel
 * task execution.
 */
class JobSystem {
 public:
  /**
   * @brief Returns the singleton instance of the JobSystem.
   * @return Reference to the JobSystem.
   */
  static JobSystem& Get() {
    static JobSystem instance;
    return instance;
  }

  /**
   * @brief Initializes the job system and spawns worker threads.
   *
   * The number of threads is determined by the system's hardware concurrency.
   */
  void Init();

  /**
   * @brief Shuts down the job system and joins all worker threads.
   */
  void Shutdown();

  /**
   * @brief Submits a task to be executed asynchronously by a worker thread.
   *
   * @tparam F The type of the callable task.
   * @param f The callable task to execute.
   * @return A std::future that will contain the result of the task.
   */
  template <typename F>
  auto Execute(F&& f) -> std::future<decltype(f())> {
    using ReturnType = decltype(f());
    auto task =
        std::make_shared<std::packaged_task<ReturnType()>>(std::forward<F>(f));
    std::future<ReturnType> res = task->get_future();
    {
      std::lock_guard<std::mutex> lock(queue_mutex_);
      if (stop_) {
        LOG_WARN("JobSystem::Execute called after shutdown. Task ignored.");
        return std::future<ReturnType>();
      }
      busy_tasks_++;
      tasks_.emplace([this, task]() {
        (*task)();
        {
          std::lock_guard<std::mutex> lock(queue_mutex_);
          busy_tasks_--;
        }
        wait_condition_.notify_all();
      });
    }
    condition_.notify_one();
    return res;
  }

  /**
   * @brief Blocks until all submitted tasks have completed.
   */
  void Wait();

  /**
   * @brief Checks if the current thread is the thread that initialized the
   * JobSystem.
   * @return True if the current thread is the main thread, false otherwise.
   */
  [[nodiscard]] bool IsMainThread() const;

 private:
  JobSystem() = default;
  ~JobSystem();

  // Prevent copy and move
  JobSystem(const JobSystem&) = delete;
  JobSystem& operator=(const JobSystem&) = delete;

  /**
   * @brief The main loop for worker threads.
   */
  void WorkerLoop();

  std::vector<std::thread> workers_;
  std::queue<std::function<void()>> tasks_;

  std::mutex queue_mutex_;
  std::condition_variable condition_;
  std::condition_variable wait_condition_;
  bool stop_ = false;
  size_t busy_tasks_ = 0;

  std::thread::id main_thread_id_;
};

}  // namespace engine::core

/**
 * @brief Macro to ensure a function is called from the main thread.
 * Fails in debug builds if called from any other thread.
 */
#ifdef NDEBUG
#define ASSERT_MAIN_THREAD() ((void)0)
#else
#include <cstdlib>

#define ASSERT_MAIN_THREAD()                                                  \
  do {                                                                        \
    if (!engine::core::JobSystem::Get().IsMainThread()) {                     \
      LOG_ERR(                                                                \
          "Assertion failed: Function must be called from the main thread."); \
      std::abort();                                                           \
    }                                                                         \
  } while (0)
#endif

#endif  // INCLUDE_ENGINE_CORE_JOB_SYSTEM_H_
