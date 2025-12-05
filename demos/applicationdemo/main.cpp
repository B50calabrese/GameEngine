#include <chrono>
#include <iostream>
#include <thread>

#include "application.h"
#include "engine.h"

class MyApp : public engine::Application {
 public:
  void OnInit() override {
    std::cout << "Initializing Application" << std::endl;
  }

  void OnShutdown() override {
    std::cout << "Shutting down Application" << std::endl;
  }

  void OnUpdate(double deltaTimeSeconds) override {
    this->totalTime_ = this->totalTime_ + deltaTimeSeconds;
    if (this->totalTime_ >= 1.0) {
      std::cout << "1 second has passed in the application." << std::endl;
      this->totalTime_ = 0.0;
    }

    // Sleep to simulate 'work'.
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

 private:
  double totalTime_ = 0.0;
};

/**
 * Demo showing the full end-to-end management of an application.
 */
int main(void) {
  engine::EngineConfig engine_config;
  engine::Engine::Init(engine_config);
  MyApp myApp;
  myApp.Run();
  return 0;
}