#include <gtest/gtest.h>

#include <atomic>
#include <chrono>
#include <future>
#include <thread>
#include <vector>

#include <engine/core/job_system.h>

namespace engine::core {

class JobSystemTest : public ::testing::Test {
 protected:
  void SetUp() override { JobSystem::Get().Init(); }

  void TearDown() override { JobSystem::Get().Shutdown(); }
};

TEST_F(JobSystemTest, ExecuteSingleTask) {
  auto future = JobSystem::Get().Execute([]() { return 42; });
  EXPECT_EQ(future.get(), 42);
}

TEST_F(JobSystemTest, ExecuteMultipleTasks) {
  std::atomic<int> counter{0};
  const int num_tasks = 100;
  std::vector<std::future<void>> futures;

  for (int i = 0; i < num_tasks; ++i) {
    futures.push_back(JobSystem::Get().Execute([&counter]() { counter++; }));
  }

  for (auto& f : futures) {
    f.get();
  }

  EXPECT_EQ(counter.load(), num_tasks);
}

TEST_F(JobSystemTest, WaitFunctionality) {
  std::atomic<int> counter{0};
  const int num_tasks = 50;

  for (int i = 0; i < num_tasks; ++i) {
    JobSystem::Get().Execute([&counter]() {
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
      counter++;
    });
  }

  JobSystem::Get().Wait();
  EXPECT_EQ(counter.load(), num_tasks);
}

TEST_F(JobSystemTest, IsMainThread) {
  // Since the test runs on the thread that initialized the JobSystem (in
  // SetUp), IsMainThread() should return true.
  EXPECT_TRUE(JobSystem::Get().IsMainThread());

  auto future = JobSystem::Get().Execute(
      []() { return JobSystem::Get().IsMainThread(); });
  // Inside a worker thread, IsMainThread() should return false.
  EXPECT_FALSE(future.get());
}

}  // namespace engine::core
