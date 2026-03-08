#include <engine/core/application.h>
#include <engine/core/engine.h>
#include <engine/util/logger.h>

class MyApp : public engine::Application {
 public:
  void OnInit() override { LOG_INFO("Initializing Application"); }

  void OnShutdown() override { LOG_INFO("Shutting down Application"); }

  void OnUpdate(double delta_time_seconds) override {
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
  engine::EngineConfig engine_config;
  engine::Engine::Init(engine_config);
  MyApp my_app;
  my_app.Run();
  return 0;
}
