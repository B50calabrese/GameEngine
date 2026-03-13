#include <vector>

#include <engine/core/application.h>
#include <engine/core/engine.h>
#include <engine/graphics/renderer.h>
#include <engine/util/logger.h>
#include <engine/util/tween_manager.h>

using namespace engine;
using namespace engine::util;
using namespace engine::graphics;

struct DemoSquare {
  glm::vec2 pos;
  glm::vec4 color;
  float rotation;
  std::string name;
};

class TweenDemoApp : public Application {
 public:
  void OnInit() override {
    LOG_INFO("TweenDemoApp Init");

    // 1. Moving Square with OutBounce
    squares_.push_back(
        {{50.0f, 500.0f}, {1.0f, 0.0f, 0.0f, 1.0f}, 0.0f, "OutBounce"});
    TweenManager::Get()
        .Tween<glm::vec2>({50.0f, 500.0f}, {750.0f, 500.0f}, 3.0f)
        .Ease(EaseType::OutBounce)
        .Loop()
        .OnUpdate([this](const glm::vec2& val, float progress) {
          squares_[0].pos = val;
        })
        .Play();

    // 2. Ping-ponging Square with InOutQuad
    squares_.push_back({{50.0f, 400.0f},
                        {0.0f, 1.0f, 0.0f, 1.0f},
                        0.0f,
                        "PingPong InOutQuad"});
    TweenManager::Get()
        .Tween<glm::vec2>({50.0f, 400.0f}, {750.0f, 400.0f}, 2.0f)
        .Ease(EaseType::InOutQuad)
        .PingPong()
        .OnUpdate([this](const glm::vec2& val, float progress) {
          squares_[1].pos = val;
        })
        .Play();

    // 3. Rotating and Scaling color Square with OutElastic
    squares_.push_back({{400.0f, 300.0f},
                        {0.0f, 0.0f, 1.0f, 1.0f},
                        0.0f,
                        "Elastic Rotate/Color"});
    TweenManager::Get()
        .Tween<float>(0.0f, 360.0f, 4.0f)
        .Ease(EaseType::OutElastic)
        .Loop()
        .OnUpdate([this](const float& val, float progress) {
          squares_[2].rotation = val;
        })
        .Play();

    TweenManager::Get()
        .Tween<glm::vec4>({0.0f, 0.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 0.0f, 1.0f},
                          2.0f)
        .Ease(EaseType::Linear)
        .PingPong()
        .OnUpdate([this](const glm::vec4& val, float progress) {
          squares_[2].color = val;
        })
        .Play();

    // 4. Delayed tween
    squares_.push_back(
        {{50.0f, 200.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, 0.0f, "Delayed InSine"});
    TweenManager::Get()
        .Tween<glm::vec2>({50.0f, 200.0f}, {750.0f, 200.0f}, 2.0f)
        .Ease(EaseType::InSine)
        .Delay(2.0f)
        .Loop()
        .OnUpdate([this](const glm::vec2& val, float progress) {
          squares_[3].pos = val;
        })
        .Play();

    // 5. Chained tweens
    squares_.push_back(
        {{50.0f, 100.0f}, {0.0f, 1.0f, 1.0f, 1.0f}, 0.0f, "Chained"});
    StartChainedTween();
  }

  void StartChainedTween() {
    TweenManager::Get()
        .Tween<glm::vec2>({50.0f, 100.0f}, {750.0f, 100.0f}, 1.5f)
        .Ease(EaseType::InOutCubic)
        .OnUpdate([this](const glm::vec2& val, float progress) {
          squares_[4].pos = val;
        })
        .OnComplete([this]() {
          TweenManager::Get()
              .Tween<glm::vec2>({750.0f, 100.0f}, {50.0f, 100.0f}, 1.5f)
              .Ease(EaseType::InOutCubic)
              .OnUpdate([this](const glm::vec2& val, float progress) {
                squares_[4].pos = val;
              })
              .OnComplete([this]() { StartChainedTween(); })
              .Play();
        })
        .Play();
  }

  void OnShutdown() override { TweenManager::Get().Clear(); }

  void OnUpdate(double delta_time_seconds) override {
    TweenManager::Get().Update(static_cast<float>(delta_time_seconds));

    for (const auto& square : squares_) {
      Renderer::Get().DrawQuad(square.pos, {50.0f, 50.0f}, square.color,
                               square.rotation);
    }
  }

 private:
  std::vector<DemoSquare> squares_;
};

int main() {
  EngineConfig config;
  config.window_width = 800;
  config.window_height = 600;
  // We don't really need assets for this demo but let's set it.
  config.asset_path = "./";

  Engine::Init(config);
  TweenDemoApp app;
  app.Run();
  return 0;
}
