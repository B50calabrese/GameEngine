/**
 * @file demo_utils.h
 * @brief Common utilities for engine demos.
 */

#ifndef DEMOS_COMMON_DEMO_UTILS_H_
#define DEMOS_COMMON_DEMO_UTILS_H_

#include <engine/core/application.h>
#include <engine/core/engine.h>
#include <engine/graphics/text_renderer.h>
#include <engine/util/tween_manager.h>

namespace demos::common {

/**
 * @brief Base class for demos to reduce boilerplate.
 *
 * Automatically handles common demo initialization like loading a default font
 * and updating the tween manager.
 */
class BaseDemoApp : public engine::Application {
 public:
  void OnInit() override final;
  void OnUpdate(double dt) override final;
  void OnShutdown() override final;

  /** @brief Custom initialization for the demo. */
  virtual void OnDemoInit() {}
  /** @brief Custom update logic for the demo. */
  virtual void OnDemoUpdate(double dt) {}
  /** @brief Custom shutdown logic for the demo. */
  virtual void OnDemoShutdown() {}
};

/**
 * @brief Static helper to run a demo application.
 */
class DemoRunner {
 public:
  /**
   * @brief Initializes the engine and runs the specified application.
   * @tparam T The application class to run (must inherit from
   * engine::Application).
   * @param config The engine configuration.
   * @return Exit code.
   */
  template <typename T>
  static int Run(const engine::EngineConfig& config) {
    engine::Engine::Init(config);
    T app;
    app.Run();
    return 0;
  }
};

}  // namespace demos::common

#endif  // DEMOS_COMMON_DEMO_UTILS_H_
