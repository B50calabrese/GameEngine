#include <engine/util/logger.h>

#include "../common/demo_utils.h"

class MyApp : public demos::common::BaseDemoApp {
 public:
  void OnDemoInit() override { LOG_INFO("Initializing Application"); }

  void OnDemoShutdown() override { LOG_INFO("Shutting down Application"); }

  void OnDemoUpdate(double delta_time_seconds) override {
    this->total_time_ = this->total_time_ + delta_time_seconds;
    if (this->total_time_ >= 1.0) {
      LOG_INFO("1 second has passed in the application.");
      this->total_time_ = 0.0;
    }
  }

 private:
  double total_time_ = 0.0;
};

/**
 * @brief Demo showing the full end-to-end management of an application.
 */
int main(void) {
  engine::EngineConfig config;
  return demos::common::DemoRunner::Run<MyApp>(config);
}
