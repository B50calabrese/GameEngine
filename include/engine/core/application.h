/**
 * @file application.h
 * @brief Defines the base Application class for the engine.
 */

#ifndef INCLUDE_ENGINE_CORE_APPLICATION_H_
#define INCLUDE_ENGINE_CORE_APPLICATION_H_

#include <memory>

#include <engine/core/engine.h>
#include <engine/core/window.h>
#include <engine/graphics/camera.h>
#include <engine/input/input_manager.h>

namespace engine {

/**
 * @brief Represents the client application that runs on top of the engine.
 *
 * The client is expected to subclass `Application` and override the lifecycle
 * methods.
 */
class Application {
 public:
  /** @brief Initializes the application instance. */
  Application();

  /** @brief Gets the singleton instance of the application. */
  static Application& Get();

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
   * @param delta_time_seconds The time elapsed since the last frame, in
   * seconds. This value should be used for frame-rate independent physics and
   *        animations.
   */
  virtual void OnUpdate(double delta_time_seconds) = 0;

  /** @brief Virtual destructor is crucial for proper inheritance cleanup. */
  virtual ~Application() = default;

  /** @brief Gets the primary camera. */
  engine::graphics::Camera& GetCamera() { return *main_camera_; }

 protected:
  /**
   * @brief Provides access to the main application window.
   * @return Reference to the Window.
   */
  Window& GetWindow() { return Engine::GetWindow(); }

  /**
   * @brief Provides access to the input manager for handling keyboard and mouse
   * events.
   * @return Reference to the InputManager.
   */
  InputManager& GetInputManager() { return InputManager::Get(); }

  /**
   * @brief Provides access to the primary camera used for rendering the scene.
   * @return Reference to the Camera.
   */
  engine::graphics::Camera& GetMainCamera() { return *main_camera_; }

 private:
  /** @brief Initializes the ImGui library. */
  void InitImGui();

  /** @brief Shuts down the ImGui library. */
  void ShutdownImGui();

  /** @brief Performs one frame of updates and rendering. */
  void ProcessFrame();

  /**
   * @brief Updates all engine-level systems.
   * @param dt Delta time in seconds.
   */
  void UpdateSystems(float dt);

  /** @brief Renders all engine-level systems. */
  void RenderSystems();

  /** @brief The Application owns the primary camera. */
  std::unique_ptr<engine::graphics::Camera> main_camera_;

  /** @brief Pointer to the global application instance. */
  static Application* instance_;
};

}  // namespace engine

#endif  // INCLUDE_ENGINE_CORE_APPLICATION_H_
