#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include <engine/core/application.h>
#include <engine/core/engine.h>
#include <engine/ecs/registry.h>
#include <engine/graphics/graphics_components.h>
#include <engine/graphics/post_processor.h>
#include <engine/graphics/renderer.h>
#include <engine/graphics/text_renderer.h>
#include <engine/input/action_manager.h>
#include <engine/input/input_manager.h>
#include <engine/scene/scene.h>
#include <engine/scene/scene_manager.h>
#include <engine/ui/ui_components.h>
#include <engine/ui/ui_systems.h>
#include <engine/util/easing.h>
#include <engine/util/tween_manager.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

using namespace engine;

// --- Components ---

struct PlayerComponent {
  glm::vec2 position = {0.0f, 0.0f};
  glm::vec2 velocity = {0.0f, 0.0f};
  bool is_grounded = false;
  float jump_force = 600.0f;
  float move_speed = 300.0f;
  glm::vec2 size = {30.0f, 30.0f};
};

enum class PlatformType { Stationary, Moving, Temporary };

struct PlatformComponent {
  PlatformType type = PlatformType::Stationary;
  glm::vec2 position;
  glm::vec2 size;
  glm::vec2 velocity = {0.0f, 0.0f};
  glm::vec2 start_pos;
  glm::vec2 end_pos;
  float timer = 0.0f;
  bool touched = false;
  bool active = true;
};

struct EnemyComponent {
  glm::vec2 position;
  glm::vec2 size = {30.0f, 30.0f};
  glm::vec2 velocity = {100.0f, 0.0f};
  glm::vec2 start_pos;
  glm::vec2 end_pos;
  bool is_patrolling = false;
};

struct GoalComponent {
  glm::vec2 position;
  glm::vec2 size = {40.0f, 40.0f};
};

// --- Helper Functions ---

bool CheckCollision(const glm::vec2& pos1, const glm::vec2& size1,
                    const glm::vec2& pos2, const glm::vec2& size2) {
  return pos1.x < pos2.x + size2.x && pos1.x + size1.x > pos2.x &&
         pos1.y < pos2.y + size2.y && pos1.y + size1.y > pos2.y;
}

// --- Scenes ---

class LevelCompleteScene : public Scene {
 public:
  LevelCompleteScene(const std::string& name, int level)
      : Scene(name), level_(level) {}

  void OnAttach() override {
    graphics::TextRenderer::Get().Init();
    graphics::TextRenderer::Get().LoadFont("default", "arial.ttf", 24);
  }

  void OnUpdate(float dt) override;

  void OnRender() override {
    graphics::Renderer::Get().DrawQuad({0.0f, 0.0f}, {800.0f, 600.0f},
                                       {0.0f, 0.0f, 0.0f, 0.8f});
    std::string msg = "LEVEL " + std::to_string(level_) + " COMPLETE!";
    graphics::Renderer::Get().DrawText("default", msg, {200.0f, 400.0f}, 0.0f,
                                       2.0f, {1.0f, 1.0f, 0.0f, 1.0f});
    graphics::Renderer::Get().DrawText("default", "Press SPACE to Continue",
                                       {250.0f, 300.0f}, 0.0f, 1.0f,
                                       {1.0f, 1.0f, 1.0f, 1.0f});
    graphics::Renderer::Get().Flush();
  }

 private:
  int level_;
};

class GameplayScene : public Scene {
 public:
  GameplayScene(const std::string& name, int level)
      : Scene(name), level_(level) {}

  void OnAttach() override {
    graphics::TextRenderer::Get().Init();
    graphics::TextRenderer::Get().LoadFont("default", "arial.ttf", 24);
    LoadLevel(level_);
  }

  void LoadLevel(int level) {
    registry().Clear();
    camera_x_ = 0.0f;

    // Create Player
    player_entity_ = registry().CreateEntity();
    PlayerComponent pc;
    pc.position = {50.0f, 100.0f};
    registry().AddComponent(player_entity_, pc);

    if (level == 1) {
      // Level 1: Basic
      CreatePlatform({0.0f, 0.0f}, {600.0f, 40.0f}, PlatformType::Stationary);
      CreatePlatform({700.0f, 0.0f}, {800.0f, 40.0f}, PlatformType::Stationary);
      CreatePlatform({250.0f, 120.0f}, {150.0f, 20.0f},
                     PlatformType::Stationary);
      CreatePlatform({500.0f, 220.0f}, {150.0f, 20.0f},
                     PlatformType::Stationary);
      CreatePlatform({800.0f, 120.0f}, {150.0f, 20.0f},
                     PlatformType::Stationary);
      CreatePlatform({1050.0f, 220.0f}, {150.0f, 20.0f},
                     PlatformType::Stationary);

      CreateEnemy({400.0f, 40.0f}, false);
      CreateEnemy({900.0f, 40.0f}, false);

      CreateGoal({1400.0f, 40.0f});
    } else if (level == 2) {
      // Level 2: Moving Platforms & Patrolling Enemies
      CreatePlatform({0.0f, 0.0f}, {300.0f, 40.0f}, PlatformType::Stationary);

      // Horizontal Moving Platform
      CreatePlatform({400.0f, 100.0f}, {150.0f, 20.0f}, PlatformType::Moving,
                     {400.0f, 100.0f}, {700.0f, 100.0f});

      CreatePlatform({800.0f, 0.0f}, {400.0f, 40.0f}, PlatformType::Stationary);
      CreateEnemy({900.0f, 40.0f}, true, {850.0f, 40.0f}, {1150.0f, 40.0f});

      // Vertical Moving Platform
      CreatePlatform({1300.0f, 100.0f}, {150.0f, 20.0f}, PlatformType::Moving,
                     {1300.0f, 100.0f}, {1300.0f, 400.0f});

      CreatePlatform({1500.0f, 0.0f}, {500.0f, 40.0f},
                     PlatformType::Stationary);
      CreateEnemy({1600.0f, 40.0f}, true, {1550.0f, 40.0f}, {1950.0f, 40.0f});

      CreateGoal({1900.0f, 40.0f});
    } else {
      // Level 3: Temporary Platforms
      CreatePlatform({0.0f, 0.0f}, {200.0f, 40.0f}, PlatformType::Stationary);

      float x = 250.0f;
      for (int i = 0; i < 5; ++i) {
        CreatePlatform({x, 100.0f + (i % 2) * 100.0f}, {100.0f, 20.0f},
                       PlatformType::Temporary);
        x += 200.0f;
      }

      CreatePlatform({1300.0f, 0.0f}, {400.0f, 40.0f},
                     PlatformType::Stationary);
      CreateEnemy({1400.0f, 40.0f}, true, {1300.0f, 40.0f}, {1650.0f, 40.0f});

      // Mix
      CreatePlatform({1800.0f, 100.0f}, {150.0f, 20.0f}, PlatformType::Moving,
                     {1800.0f, 100.0f}, {2100.0f, 100.0f});
      CreatePlatform({2200.0f, 200.0f}, {100.0f, 20.0f},
                     PlatformType::Temporary);
      CreatePlatform({2400.0f, 300.0f}, {100.0f, 20.0f},
                     PlatformType::Temporary);

      CreatePlatform({2600.0f, 0.0f}, {400.0f, 40.0f},
                     PlatformType::Stationary);
      CreateEnemy({2700.0f, 40.0f}, false);
      CreateEnemy({2800.0f, 40.0f}, false);

      CreateGoal({2900.0f, 40.0f});
    }
  }

  void CreatePlatform(glm::vec2 pos, glm::vec2 size, PlatformType type,
                      glm::vec2 start = {0, 0}, glm::vec2 end = {0, 0}) {
    auto entity = registry().CreateEntity();
    PlatformComponent pc;
    pc.type = type;
    pc.position = pos;
    pc.size = size;
    pc.start_pos = start;
    pc.end_pos = end;
    if (type == PlatformType::Moving) {
      pc.velocity = glm::normalize(end - start) * 150.0f;
    }
    registry().AddComponent(entity, pc);
  }

  void CreateEnemy(glm::vec2 pos, bool patrolling, glm::vec2 start = {0, 0},
                   glm::vec2 end = {0, 0}) {
    auto entity = registry().CreateEntity();
    EnemyComponent ec;
    ec.position = pos;
    ec.is_patrolling = patrolling;
    ec.start_pos = start;
    ec.end_pos = end;
    if (patrolling) {
      ec.velocity = glm::normalize(end - start) * 100.0f;
    }
    registry().AddComponent(entity, ec);
  }

  void CreateGoal(glm::vec2 pos) {
    auto entity = registry().CreateEntity();
    GoalComponent gc;
    gc.position = pos;
    registry().AddComponent(entity, gc);
  }

  void OnUpdate(float dt) override {
    UpdatePlatforms(dt);
    UpdatePlayer(dt);
    UpdateEnemies(dt);
    UpdateCamera();

    // Check Goal
    auto& player = registry().GetComponent<PlayerComponent>(player_entity_);
    registry().ForEach<GoalComponent>([this, &player](GoalComponent& gc) {
      if (CheckCollision(player.position, player.size, gc.position, gc.size)) {
        SceneManager::Get().PushScene(
            std::make_unique<LevelCompleteScene>("Complete", level_));
      }
    });
  }

  void UpdatePlatforms(float dt) {
    registry().ForEach<PlatformComponent>([dt](PlatformComponent& pc) {
      if (!pc.active) return;

      if (pc.type == PlatformType::Moving) {
        pc.position += pc.velocity * dt;
        if (glm::distance(pc.position, pc.start_pos) < 5.0f)
          pc.velocity = glm::normalize(pc.end_pos - pc.start_pos) * 150.0f;
        if (glm::distance(pc.position, pc.end_pos) < 5.0f)
          pc.velocity = glm::normalize(pc.start_pos - pc.end_pos) * 150.0f;
      }

      if (pc.type == PlatformType::Temporary && pc.touched) {
        pc.timer += dt;
        if (pc.timer > 2.0f) pc.active = false;
      }
    });
  }

  void UpdatePlayer(float dt) {
    auto& player = registry().GetComponent<PlayerComponent>(player_entity_);

    // Gravity
    player.velocity.y -= 1500.0f * dt;

    // Input
    if (InputManager::Get().IsKeyDown(KeyCode::KC_LEFT))
      player.velocity.x = -player.move_speed;
    else if (InputManager::Get().IsKeyDown(KeyCode::KC_RIGHT))
      player.velocity.x = player.move_speed;
    else
      player.velocity.x = 0;

    if (InputManager::Get().IsKeyPressed(KeyCode::KC_SPACE) &&
        player.is_grounded) {
      player.velocity.y = player.jump_force;
      player.is_grounded = false;
    }

    // Move Y and resolve collisions
    player.position.y += player.velocity.y * dt;
    player.is_grounded = false;
    registry().ForEach<PlatformComponent>([&player](PlatformComponent& pc) {
      if (!pc.active) return;
      if (CheckCollision(player.position, player.size, pc.position, pc.size)) {
        if (player.velocity.y < 0) {
          player.position.y = pc.position.y + pc.size.y;
          player.velocity.y = 0;
          player.is_grounded = true;
          if (pc.type == PlatformType::Temporary) pc.touched = true;
        } else {
          player.position.y = pc.position.y - player.size.y;
          player.velocity.y = 0;
        }
      }
    });

    // Move X and resolve collisions
    player.position.x += player.velocity.x * dt;
    registry().ForEach<PlatformComponent>([&player](PlatformComponent& pc) {
      if (!pc.active) return;
      if (CheckCollision(player.position, player.size, pc.position, pc.size)) {
        if (player.velocity.x > 0)
          player.position.x = pc.position.x - player.size.x;
        else if (player.velocity.x < 0)
          player.position.x = pc.position.x + pc.size.x;
      }
    });

    // Screen boundaries
    if (player.position.x < camera_x_) player.position.x = camera_x_;
    if (player.position.y < -100.0f) ResetPlayer();  // Fell off
  }

  void UpdateEnemies(float dt) {
    auto& player = registry().GetComponent<PlayerComponent>(player_entity_);
    registry().ForEach<EnemyComponent>([this, dt, &player](EnemyComponent& ec) {
      if (ec.is_patrolling) {
        ec.position += ec.velocity * dt;
        if (glm::distance(ec.position, ec.start_pos) < 5.0f)
          ec.velocity = glm::normalize(ec.end_pos - ec.start_pos) * 100.0f;
        if (glm::distance(ec.position, ec.end_pos) < 5.0f)
          ec.velocity = glm::normalize(ec.start_pos - ec.end_pos) * 100.0f;
      }

      if (CheckCollision(player.position, player.size, ec.position, ec.size)) {
        ResetPlayer();
      }
    });
  }

  void UpdateCamera() {
    auto& player = registry().GetComponent<PlayerComponent>(player_entity_);
    float center_x = camera_x_ + 400.0f;
    if (player.position.x > center_x) {
      camera_x_ = player.position.x - 400.0f;
    }
  }

  void ResetPlayer() {
    auto& player = registry().GetComponent<PlayerComponent>(player_entity_);
    player.position = {50.0f, 100.0f};
    camera_x_ = 0.0f;
    player.velocity = {0, 0};

    // Reset temporary platforms
    registry().ForEach<PlatformComponent>([](PlatformComponent& pc) {
      if (pc.type == PlatformType::Temporary) {
        pc.active = true;
        pc.touched = false;
        pc.timer = 0.0f;
      }
    });
  }

  void OnRender() override {
    glm::vec2 offset = {-camera_x_, 0.0f};

    // Draw Background
    graphics::Renderer::Get().DrawQuad({0.0f, 0.0f}, {800.0f, 600.0f},
                                       {0.1f, 0.1f, 0.2f, 1.0f});

    // Draw Platforms
    registry().ForEach<PlatformComponent>([offset](PlatformComponent& pc) {
      if (!pc.active) return;
      glm::vec4 color;
      if (pc.type == PlatformType::Stationary)
        color = {0.4f, 0.4f, 0.4f, 1.0f};
      else if (pc.type == PlatformType::Moving)
        color = {0.2f, 0.6f, 0.8f, 1.0f};
      else
        color = {0.8f, 0.4f, 0.2f, 1.0f};
      graphics::Renderer::Get().DrawQuad(pc.position + offset, pc.size, color);
    });

    // Draw Goal
    registry().ForEach<GoalComponent>([offset](GoalComponent& gc) {
      graphics::Renderer::Get().DrawQuad(gc.position + offset, gc.size,
                                         {1.0f, 1.0f, 0.0f, 1.0f});
    });

    // Draw Enemies
    registry().ForEach<EnemyComponent>([offset](EnemyComponent& ec) {
      graphics::Renderer::Get().DrawQuad(ec.position + offset, ec.size,
                                         {1.0f, 0.0f, 0.0f, 1.0f});
    });

    // Draw Player
    auto& player = registry().GetComponent<PlayerComponent>(player_entity_);
    graphics::Renderer::Get().DrawQuad(player.position + offset, player.size,
                                       {1.0f, 1.0f, 1.0f, 1.0f});

    // Draw UI (No offset)
    graphics::Renderer::Get().DrawText(
        "default", "Level: " + std::to_string(level_), {10.0f, 570.0f}, 0.0f,
        0.8f, {1.0f, 1.0f, 1.0f, 1.0f});

    graphics::Renderer::Get().Flush();
  }

 private:
  int level_;
  ecs::EntityID player_entity_;
  float camera_x_ = 0.0f;
};

// --- MenuScene Update with Gameplay transition ---

class MenuScene : public Scene {
 public:
  MenuScene(const std::string& name) : Scene(name) {}

  void OnAttach() override {
    graphics::TextRenderer::Get().Init();
    graphics::TextRenderer::Get().LoadFont("default", "arial.ttf", 24);
  }

  void OnUpdate(float dt) override {
    if (show_controls_) {
      if (InputManager::Get().IsKeyPressed(KeyCode::KC_ESCAPE) ||
          InputManager::Get().IsKeyPressed(KeyCode::KC_SPACE) ||
          InputManager::Get().IsKeyPressed(KeyCode::KC_ENTER)) {
        show_controls_ = false;
      }
      return;
    }

    if (InputManager::Get().IsKeyPressed(KeyCode::KC_1) ||
        InputManager::Get().IsKeyPressed(KeyCode::KC_SPACE) ||
        InputManager::Get().IsKeyPressed(KeyCode::KC_ENTER)) {
      SceneManager::Get().SetScene(
          std::make_unique<GameplayScene>("Gameplay", 1));
    } else if (InputManager::Get().IsKeyPressed(KeyCode::KC_2)) {
      show_controls_ = true;
    } else if (InputManager::Get().IsKeyPressed(KeyCode::KC_3) ||
               InputManager::Get().IsKeyPressed(KeyCode::KC_ESCAPE)) {
      glfwSetWindowShouldClose(Engine::window().native_handle(), GLFW_TRUE);
    }
  }

  void OnRender() override {
    graphics::Renderer::Get().DrawQuad({0.0f, 0.0f}, {800.0f, 600.0f},
                                       {0.1f, 0.1f, 0.15f, 1.0f});

    if (show_controls_) {
      graphics::Renderer::Get().DrawText("default", "CONTROLS",
                                         {300.0f, 450.0f}, 0.0f, 1.5f,
                                         {1.0f, 1.0f, 0.0f, 1.0f});
      graphics::Renderer::Get().DrawText("default", "ARROWS: Move",
                                         {300.0f, 350.0f}, 0.0f, 1.0f,
                                         {1.0f, 1.0f, 1.0f, 1.0f});
      graphics::Renderer::Get().DrawText("default", "SPACE: Jump",
                                         {300.0f, 300.0f}, 0.0f, 1.0f,
                                         {1.0f, 1.0f, 1.0f, 1.0f});
      graphics::Renderer::Get().DrawText("default", "Press SPACE to go back",
                                         {250.0f, 150.0f}, 0.0f, 0.8f,
                                         {0.7f, 0.7f, 0.7f, 1.0f});
    } else {
      graphics::Renderer::Get().DrawText("default", "PLATFORMER DEMO",
                                         {200.0f, 450.0f}, 0.0f, 2.0f,
                                         {0.0f, 1.0f, 0.5f, 1.0f});
      graphics::Renderer::Get().DrawText("default", "1. Start Game",
                                         {300.0f, 300.0f}, 0.0f, 1.2f,
                                         {1.0f, 1.0f, 1.0f, 1.0f});
      graphics::Renderer::Get().DrawText("default", "2. Controls",
                                         {300.0f, 250.0f}, 0.0f, 1.2f,
                                         {1.0f, 1.0f, 1.0f, 1.0f});
      graphics::Renderer::Get().DrawText("default", "3. Exit", {300.0f, 200.0f},
                                         0.0f, 1.2f, {1.0f, 1.0f, 1.0f, 1.0f});
    }
    graphics::Renderer::Get().Flush();
  }

 private:
  bool show_controls_ = false;
};

void LevelCompleteScene::OnUpdate(float dt) {
  if (InputManager::Get().IsKeyPressed(KeyCode::KC_SPACE) ||
      InputManager::Get().IsKeyPressed(KeyCode::KC_ENTER)) {
    if (level_ < 3) {
      SceneManager::Get().SetScene(
          std::make_unique<GameplayScene>("Gameplay", level_ + 1));
    } else {
      SceneManager::Get().SetScene(std::make_unique<MenuScene>("Menu"));
    }
  }
}

class PlatformerApp : public Application {
 public:
  void OnInit() override {
    SceneManager::Get().SetScene(std::make_unique<MenuScene>("Menu"));
  }
  void OnShutdown() override {}
  void OnUpdate(double dt) override {}
};

int main(void) {
  EngineConfig config;
  config.window_width = 800;
  config.window_height = 600;
  config.asset_path = ENGINE_ASSETS_PATH;
  Engine::Init(config);
  PlatformerApp app;
  app.Run();
  return 0;
}
