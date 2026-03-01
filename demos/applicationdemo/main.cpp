#include <iostream>

#include "application.h"
#include "engine.h"

class MyApp : public engine::Application {
 public:
  void OnInit() override { std::cout << "Initializing Application" << std::endl; }

  void OnShutdown() override {
    std::cout << "Shutting down Application" << std::endl;
  }

  void OnUpdate(double delta_time_seconds) override {
    this->total_time_ = this->total_time_ + delta_time_seconds;
    if (this->total_time_ >= 1.0) {
      std::cout << "1 second has passed in the application." << std::endl;
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
