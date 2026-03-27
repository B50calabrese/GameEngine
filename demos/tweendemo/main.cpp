#include <vector>

#include <engine/graphics/renderer.h>
#include <engine/util/logger.h>
#include <engine/util/tween_manager.h>

#include "../common/demo_utils.h"

struct DemoSquare {
  glm::vec2 pos;
  glm::vec4 color;
  float rotation;
  std::string name;
};

class TweenDemoApp : public demos::common::BaseDemoApp {
 public:
  void OnDemoInit() override {
    LOG_INFO("TweenDemoApp Init");

    // 1. Moving Square with OutBounce
    squares_.push_back(
        {{50.0f, 500.0f}, {1.0f, 0.0f, 0.0f, 1.0f}, 0.0f, "OutBounce"});
    engine::util::TweenManager::Get()
        .Tween<glm::vec2>({50.0f, 500.0f}, {750.0f, 500.0f}, 3.0f)
        .Ease(engine::util::EaseType::OutBounce)
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
    engine::util::TweenManager::Get()
        .Tween<glm::vec2>({50.0f, 400.0f}, {750.0f, 400.0f}, 2.0f)
        .Ease(engine::util::EaseType::InOutQuad)
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
    engine::util::TweenManager::Get()
        .Tween<float>(0.0f, 360.0f, 4.0f)
        .Ease(engine::util::EaseType::OutElastic)
        .Loop()
        .OnUpdate([this](const float& val, float progress) {
          squares_[2].rotation = val;
        })
        .Play();

    engine::util::TweenManager::Get()
        .Tween<glm::vec4>({0.0f, 0.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 0.0f, 1.0f},
                          2.0f)
        .Ease(engine::util::EaseType::Linear)
        .PingPong()
        .OnUpdate([this](const glm::vec4& val, float progress) {
          squares_[2].color = val;
        })
        .Play();

    // 4. Delayed tween
    squares_.push_back(
        {{50.0f, 200.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, 0.0f, "Delayed InSine"});
    engine::util::TweenManager::Get()
        .Tween<glm::vec2>({50.0f, 200.0f}, {750.0f, 200.0f}, 2.0f)
        .Ease(engine::util::EaseType::InSine)
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
    engine::util::TweenManager::Get()
        .Tween<glm::vec2>({50.0f, 100.0f}, {750.0f, 100.0f}, 1.5f)
        .Ease(engine::util::EaseType::InOutCubic)
        .OnUpdate([this](const glm::vec2& val, float progress) {
          squares_[4].pos = val;
        })
        .OnComplete([this]() {
          engine::util::TweenManager::Get()
              .Tween<glm::vec2>({750.0f, 100.0f}, {50.0f, 100.0f}, 1.5f)
              .Ease(engine::util::EaseType::InOutCubic)
              .OnUpdate([this](const glm::vec2& val, float progress) {
                squares_[4].pos = val;
              })
              .OnComplete([this]() { StartChainedTween(); })
              .Play();
        })
        .Play();
  }

  void OnDemoShutdown() override { engine::util::TweenManager::Get().Clear(); }

  void OnDemoUpdate(double delta_time_seconds) override {
    for (const auto& square : squares_) {
      engine::graphics::Renderer::Get().DrawQuad(square.pos, {50.0f, 50.0f},
                                                 square.color, square.rotation);
    }
  }

 private:
  std::vector<DemoSquare> squares_;
};

int main() {
  engine::EngineConfig config;
  config.window_width = 800;
  config.window_height = 600;
  config.asset_path = ENGINE_ASSETS_PATH;
  return demos::common::DemoRunner::Run<TweenDemoApp>(config);
}
