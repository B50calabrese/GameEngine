#ifndef INCLUDE_APPLICATION_H_
#define INCLUDE_APPLICATION_H_

#include <memory>

#include "engine.h"
#include "graphics/camera.h"
#include "input_manager.h"
#include "window.h"

namespace engine {

/**
 * @brief Represents the client application that runs on top of the engine.
 *
 * The client is expected to subclass `Application` and override the lifecycle
 * methods.
 */
class Application {
 public:
  /**
   * @brief Drives the main game loop.
   *
   * This method will continuously call `OnUpdate` until the application is
   * terminated.
   */
  void Run();

  /**
   * @brief Called once when the application starts.
   *
   * This is the ideal place for one-time initialization code, such as loading
   * assets.
   */
  virtual void OnInit() = 0;

  /**
   * @brief Called once when the application is about to close.
   *
   * Use this for cleanup code.
   */
  virtual void OnShutdown() = 0;

  /**
   * @brief Called every frame. Game logic and rendering should be handled here.
   *
   * @param deltaTimeSeconds The time elapsed since the last frame, in seconds.
   *        This value should be used for frame-rate independent physics and
   *        animations.
   */
  virtual void OnUpdate(double deltaTimeSeconds) = 0;

  /** @brief Virtual destructor is crucial for proper inheritance cleanup. */
  virtual ~Application() = default;

 protected:
  /**
   * @brief Provides access to the main application window.
   * @return Reference to the Window.
   */
  Window& window() { return Engine::window(); }

  /**
   * @brief Provides access to the input manager for handling keyboard and mouse
   * events.
   * @return Reference to the InputManager.
   */
  InputManager& input_manager() { return InputManager::Get(); }

  /**
   * @brief Provides access to the primary camera used for rendering the scene.
   * @return Reference to the Camera.
   */
  engine::graphics::Camera& main_camera() { return *main_camera_; }

 private:
  /** @brief The Application owns the primary camera. */
  std::unique_ptr<engine::graphics::Camera> main_camera_;
};

}  // namespace engine

#endif  // INCLUDE_APPLICATION_H_
