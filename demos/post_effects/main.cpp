#include <iostream>

#include <engine/core/application.h>
#include <engine/core/engine.h>
#include <engine/graphics/post_processor.h>
#include <engine/graphics/renderer.h>
#include <engine/input/action_manager.h>
#include <engine/input/input_manager.h>
#include <engine/util/tween_manager.h>

using namespace engine;
using namespace engine::graphics;
using namespace engine::util;

/** @brief Demo to showcase post-processing effects like Screen Shake and Flash.
 */
class PostEffectsDemo : public Application {
 public:
  void OnInit() override {
    // Register actions
    ActionManager::Get().BindAction("Shake", KeyCode::KC_S);
    ActionManager::Get().BindAction("Flash", KeyCode::KC_F);

    std::cout << "Post-Effects Demo Initialized" << std::endl;
    std::cout << "Press 'S' for Screen Shake" << std::endl;
    std::cout << "Press 'F' for Screen Flash" << std::endl;
  }

  void OnShutdown() override {
    std::cout << "Post-Effects Demo Shutting Down" << std::endl;
  }

  void OnUpdate(double delta_time) override {
    // 1. Handle Input
    if (ActionManager::Get().IsStarted("Shake")) {
      TweenManager::Get()
          .Tween<float>(0.05f, 0.0f, 0.5f)
          .OnUpdate([](const float& value, float progress) {
            PostProcessManager::Get().SetShake(value);
          })
          .Play();
    }

    if (ActionManager::Get().IsStarted("Flash")) {
      TweenManager::Get()
          .Tween<float>(1.0f, 0.0f, 0.2f)
          .OnUpdate([](const float& value, float progress) {
            PostProcessManager::Get().SetFlash({1.0f, 1.0f, 1.0f}, value);
          })
          .Play();
    }

    // 2. Update Tweens
    TweenManager::Get().Update(static_cast<float>(delta_time));

    // 3. Render some scene content to be post-processed
    Renderer::Get().DrawRect(100.0f, 100.0f, 200.0f, 200.0f, 1.0f, 0.0f,
                             0.0f);  // Red Box
    Renderer::Get().DrawRect(400.0f, 300.0f, 150.0f, 150.0f, 0.0f, 1.0f,
                             0.0f);  // Green Box
    Renderer::Get().DrawRect(200.0f, 400.0f, 100.0f, 100.0f, 0.0f, 0.0f,
                             1.0f);  // Blue Box
  }
};

int main() {
  PostEffectsDemo demo;
  demo.Run();
  return 0;
}
