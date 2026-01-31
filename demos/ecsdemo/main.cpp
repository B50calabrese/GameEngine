#include <iostream>
#include <memory>

#include "application.h"
#include "engine.h"
#include "graphics/renderer.h"
#include "input_manager.h"
#include "scene.h"
#include "scene_manager.h"

constexpr float WIDTH = 800.0f;
constexpr float HEIGHT = 600.0f;

class MainScene : public engine::Scene {
 public:
  MainScene(const std::string& name) : Scene(name) {}

  void OnRender() override {}

  bool OnInput() override { return true; }
};

class MyApp : public engine::Application {
 public:
  void OnInit() override {
    std::cout << "Initializing Application" << std::endl;
    engine::SceneManager::Get().PushScene(
        std::make_unique<MainScene>("SceneA"));
  }

  void OnShutdown() override {
    std::cout << "Shutting down Application" << std::endl;
  }

  void OnUpdate(double deltaTimeSeconds) override {}

 private:
  double totalTime_ = 0.0;
};

/**
 * Demo showing the usage of an entity-component-system framework.
 */
int main(void) {
  engine::EngineConfig engine_config;
  engine_config.window_height = HEIGHT;
  engine_config.window_width = WIDTH;
  engine::Engine::Init(engine_config);
  MyApp myApp;
  myApp.Run();
  return 0;
}