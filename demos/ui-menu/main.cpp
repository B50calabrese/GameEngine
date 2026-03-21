#include <iostream>
#include <memory>

#include <engine/core/application.h>
#include <engine/core/engine.h>
#include <engine/ecs/registry.h>
#include <engine/graphics/graphics_components.h>
#include <engine/graphics/renderer.h>
#include <engine/graphics/text_renderer.h>
#include <engine/input/action_manager.h>
#include <engine/input/input_manager.h>
#include <engine/scene/scene.h>
#include <engine/scene/scene_manager.h>
#include <engine/ui/ui_components.h>
#include <engine/ui/ui_systems.h>
#include <engine/util/tween_manager.h>

class MenuScene : public engine::Scene {
 public:
  MenuScene(const std::string& name) : engine::Scene(name) {
    // 1. Player
    player_ = registry().CreateEntity();

    // 2. HUD Root
    hud_root_ = registry().CreateEntity();
    engine::ui::UITransform hud_trans;
    hud_trans.local_pos = {10.0f, 550.0f};  // Top left
    hud_trans.size = {200.0f, 50.0f};
    registry().AddComponent(hud_root_, hud_trans);
    registry().AddComponent(hud_root_, engine::ui::UIHierarchy{});

    // 3. Gold Counter (Child of HUD)
    gold_counter_ = registry().CreateEntity();
    engine::ui::UITransform gold_trans;
    gold_trans.local_pos = {0.0f, 0.0f};
    gold_trans.size = {150.0f, 30.0f};
    registry().AddComponent(gold_counter_, gold_trans);

    engine::ui::UIHierarchy gold_hier;
    gold_hier.parent = hud_root_;
    registry().AddComponent(gold_counter_, gold_hier);
    registry()
        .GetComponent<engine::ui::UIHierarchy>(hud_root_)
        .children.push_back(gold_counter_);

    engine::ui::UIBinding gold_binding;
    gold_binding.get_text = [this]() {
      return "Gold: " + std::to_string(player_gold_);
    };
    registry().AddComponent(gold_counter_, gold_binding);
    registry().AddComponent(
        gold_counter_, engine::graphics::TextComponent{
                           "Gold: 0", "arial", 1.0f, {1.0f, 1.0f, 0.0f, 1.0f}});

    // 4. Pause Button
    pause_button_ = registry().CreateEntity();
    engine::ui::UITransform btn_trans;
    btn_trans.local_pos = {700.0f, 550.0f};
    btn_trans.size = {80.0f, 40.0f};
    registry().AddComponent(pause_button_, btn_trans);
    registry().AddComponent(pause_button_, engine::graphics::QuadComponent{
                                               {0.5f, 0.5f, 0.5f, 1.0f}});
    registry().AddComponent(pause_button_, engine::ui::UIHierarchy{});

    engine::ui::UIInteractable btn_inter;
    btn_inter.on_click = [this]() { ToggleMenu(); };
    btn_inter.on_hover_changed = [this](bool hovered) {
      float start_scale = hovered ? 1.0f : 1.1f;
      float end_scale = hovered ? 1.1f : 1.0f;
      engine::util::TweenManager::Get()
          .Tween<float>(start_scale, end_scale, 0.1f)
          .OnUpdate([this](const float& val, float progress) {
            auto& t =
                registry().GetComponent<engine::ui::UITransform>(pause_button_);
            t.size = {80.0f * val, 40.0f * val};
          })
          .Play();
    };
    registry().AddComponent(pause_button_, btn_inter);

    // Button Text
    engine::ecs::EntityID btn_text = registry().CreateEntity();
    engine::ui::UITransform bt_trans;
    bt_trans.local_pos = {5.0f, 10.0f};
    registry().AddComponent(btn_text, bt_trans);
    registry().AddComponent(
        btn_text, engine::graphics::TextComponent{
                      "PAUSE", "arial", 0.8f, {1.0f, 1.0f, 1.0f, 1.0f}});
    engine::ui::UIHierarchy bt_hier;
    bt_hier.parent = pause_button_;
    registry().AddComponent(btn_text, bt_hier);
    registry()
        .GetComponent<engine::ui::UIHierarchy>(pause_button_)
        .children.push_back(btn_text);

    // 5. Menu Panel (Initially off-screen)
    menu_panel_ = registry().CreateEntity();
    engine::ui::UITransform menu_trans;
    menu_trans.local_pos = {800.0f, 150.0f};  // Off-screen right
    menu_trans.size = {300.0f, 300.0f};
    registry().AddComponent(menu_panel_, menu_trans);
    registry().AddComponent(
        menu_panel_, engine::graphics::QuadComponent{{0.2f, 0.2f, 0.2f, 0.9f}});
    registry().AddComponent(menu_panel_, engine::ui::UIHierarchy{});

    engine::ecs::EntityID menu_text = registry().CreateEntity();
    engine::ui::UITransform mt_trans;
    mt_trans.local_pos = {50.0f, 250.0f};
    registry().AddComponent(menu_text, mt_trans);
    registry().AddComponent(
        menu_text, engine::graphics::TextComponent{
                       "PAUSED", "arial", 1.5f, {1.0f, 1.0f, 1.0f, 1.0f}});
    engine::ui::UIHierarchy mt_hier;
    mt_hier.parent = menu_panel_;
    registry().AddComponent(menu_text, mt_hier);
    registry()
        .GetComponent<engine::ui::UIHierarchy>(menu_panel_)
        .children.push_back(menu_text);
  }

  void OnAttach() override {
    engine::graphics::TextRenderer::Get().Init();
    engine::graphics::TextRenderer::Get().LoadFont("arial", "arial.ttf", 24);
  }

  void OnUpdate(float delta_time_seconds) override {
    engine::util::TweenManager::Get().Update(delta_time_seconds);

    if (!engine::ActionManager::Get().IsConsumed()) {
      if (engine::InputManager::Get().IsKeyDown(engine::KeyCode::kW))
        player_pos_.y += 200.0f * delta_time_seconds;
      if (engine::InputManager::Get().IsKeyDown(engine::KeyCode::kS))
        player_pos_.y -= 200.0f * delta_time_seconds;
      if (engine::InputManager::Get().IsKeyDown(engine::KeyCode::kA))
        player_pos_.x -= 200.0f * delta_time_seconds;
      if (engine::InputManager::Get().IsKeyDown(engine::KeyCode::kD))
        player_pos_.x += 200.0f * delta_time_seconds;

      if (engine::InputManager::Get().IsKeyPressed(engine::KeyCode::kSpace)) {
        player_gold_ += 10;
      }
    }

    auto& btn_inter =
        registry().GetComponent<engine::ui::UIInteractable>(pause_button_);
    auto& btn_quad =
        registry().GetComponent<engine::graphics::QuadComponent>(pause_button_);
    if (btn_inter.is_hovered) {
      btn_quad.color = {0.7f, 0.7f, 0.7f, 1.0f};
    } else {
      btn_quad.color = {0.5f, 0.5f, 0.5f, 1.0f};
    }
  }

  void OnRender() override {
    engine::graphics::Renderer::Get().DrawQuad(player_pos_, {32.0f, 32.0f},
                                               {0.0f, 1.0f, 0.0f, 1.0f});
    engine::graphics::Renderer::Get().DrawText(
        "arial", "Use WASD to move, Space to get gold", {10.0f, 10.0f}, 0.0f,
        0.7f, {1.0f, 1.0f, 1.0f, 1.0f});
    if (engine::ActionManager::Get().IsConsumed()) {
      engine::graphics::Renderer::Get().DrawText(
          "arial", "INPUT CONSUMED BY UI", {300.0f, 550.0f}, 0.0f, 0.8f,
          {1.0f, 0.0f, 0.0f, 1.0f});
    }
    engine::graphics::Renderer::Get().Flush();
  }

  void ToggleMenu() {
    is_menu_open_ = !is_menu_open_;
    float target_x = is_menu_open_ ? 250.0f : 800.0f;
    auto& trans = registry().GetComponent<engine::ui::UITransform>(menu_panel_);
    float start_x = trans.local_pos.x;
    engine::util::TweenManager::Get()
        .Tween<float>(start_x, target_x, 0.5f)
        .Ease(engine::util::EaseType::OutCubic)
        .OnUpdate([this](const float& val, float progress) {
          auto& t =
              registry().GetComponent<engine::ui::UITransform>(menu_panel_);
          t.local_pos.x = val;
          t.is_dirty = true;
        })
        .Play();
  }

 private:
  engine::ecs::EntityID player_;
  engine::ecs::EntityID hud_root_;
  engine::ecs::EntityID gold_counter_;
  engine::ecs::EntityID pause_button_;
  engine::ecs::EntityID menu_panel_;
  glm::vec2 player_pos_ = {400.0f, 300.0f};
  int player_gold_ = 0;
  bool is_menu_open_ = false;
};

class MyApp : public engine::Application {
 public:
  void OnInit() override {
    engine::SceneManager::Get().SetScene(
        std::make_unique<MenuScene>("UI Menu Demo"));
  }
  void OnShutdown() override {}
  void OnUpdate(double delta_time_seconds) override {}
};

int main(void) {
  engine::EngineConfig config;
  config.window_width = 800;
  config.window_height = 600;
  config.asset_path = ENGINE_ASSETS_PATH;
  engine::Engine::Init(config);
  MyApp app;
  app.Run();
  return 0;
}
