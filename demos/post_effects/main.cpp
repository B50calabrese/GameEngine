#include <iostream>

#include <engine/core/application.h>
#include <engine/core/engine.h>
#include <engine/graphics/post_processor.h>
#include <engine/graphics/renderer.h>
#include <engine/input/action_manager.h>
#include <engine/input/input_manager.h>
#include <engine/util/tween_manager.h>





/** @brief Demo to showcase post-processing effects like Screen Shake and Flash.
 */
class PostEffectsDemo : public engine::Application {
 public:
  void OnInit() override {
    // Register actions
    engine::ActionManager::Get().BindAction("Shake", engine::KeyCode::kS);
    engine::ActionManager::Get().BindAction("Flash", engine::KeyCode::kF);

    std::cout << "Post-Effects Demo Initialized" << std::endl;
    std::cout << "Press 'S' for Screen Shake" << std::endl;
    std::cout << "Press 'F' for Screen Flash" << std::endl;
  }

  void OnShutdown() override {
    std::cout << "Post-Effects Demo Shutting Down" << std::endl;
  }

  void OnUpdate(double delta_time) override {
    // 1. Handle Input
    if (engine::ActionManager::Get().IsStarted("Shake")) {
      engine::util::TweenManager::Get()
          .Tween<float>(0.05f, 0.0f, 0.5f)
          .OnUpdate([](const float& value, float progress) {
            engine::graphics::PostProcessManager::Get().SetShake(value);
          })
          .Play();
    }

    if (engine::ActionManager::Get().IsStarted("Flash")) {
      engine::util::TweenManager::Get()
          .Tween<float>(1.0f, 0.0f, 0.2f)
          .OnUpdate([](const float& value, float progress) {
            engine::graphics::PostProcessManager::Get().SetFlash({1.0f, 1.0f, 1.0f}, value);
          })
          .Play();
    }

    // 2. Update Tweens
    engine::util::TweenManager::Get().Update(static_cast<float>(delta_time));

    // 3. Render some scene content to be post-processed
    engine::graphics::Renderer::Get().DrawRect(100.0f, 100.0f, 200.0f, 200.0f, 1.0f, 0.0f,
                             0.0f);  // Red Box
    engine::graphics::Renderer::Get().DrawRect(400.0f, 300.0f, 150.0f, 150.0f, 0.0f, 1.0f,
                             0.0f);  // Green Box
    engine::graphics::Renderer::Get().DrawRect(200.0f, 400.0f, 100.0f, 100.0f, 0.0f, 0.0f,
                             1.0f);  // Blue Box
  }
};

int main() {
  engine::EngineConfig config;
  config.window_width = 800;
  config.window_height = 600;
  engine::Engine::Init(config);
  PostEffectsDemo demo;
  demo.Run();
  return 0;
}
