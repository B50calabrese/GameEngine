#include <memory>

#include <engine/ecs/components/graphics_components.h>
#include <engine/ecs/registry.h>
#include <engine/graphics/renderer.h>
#include <engine/input/action_manager.h>
#include <engine/input/input_manager.h>
#include <engine/scene/scene.h>
#include <engine/scene/scene_manager.h>
#include <engine/ui/ui_components.h>
#include <engine/ui/ui_systems.h>
#include <engine/util/tween_manager.h>

#include "../common/demo_utils.h"

class MenuScene : public engine::Scene {
 public:
  MenuScene(const std::string& name) : engine::Scene(name) {
    // 1. Player
    player_ = GetRegistry().CreateEntity();

    // 2. HUD Root
    hud_root_ = GetRegistry().CreateEntity();
    engine::ui::UITransform hud_trans;
    hud_trans.local_pos = {10.0f, 550.0f};  // Top left
    hud_trans.size = {200.0f, 50.0f};
    GetRegistry().AddComponent(hud_root_, hud_trans);
    GetRegistry().AddComponent(hud_root_, engine::ui::UIHierarchy{});

    // 3. Gold Counter (Child of HUD)
    gold_counter_ = GetRegistry().CreateEntity();
    engine::ui::UITransform gold_trans;
    gold_trans.local_pos = {0.0f, 0.0f};
    gold_trans.size = {150.0f, 30.0f};
    GetRegistry().AddComponent(gold_counter_, gold_trans);

    engine::ui::UIHierarchy gold_hier;
    gold_hier.parent = hud_root_;
    GetRegistry().AddComponent(gold_counter_, gold_hier);
    GetRegistry()
        .GetComponent<engine::ui::UIHierarchy>(hud_root_)
        .children.push_back(gold_counter_);

    engine::ui::UIBinding gold_binding;
    gold_binding.get_text = [this]() {
      return "Gold: " + std::to_string(player_gold_);
    };
    GetRegistry().AddComponent(gold_counter_, gold_binding);
    GetRegistry().AddComponent(
        gold_counter_,
        engine::ecs::components::Text{
            "Gold: 0", "default", 1.0f, {1.0f, 1.0f, 0.0f, 1.0f}});

    // 4. Pause Button
    pause_button_ = GetRegistry().CreateEntity();
    engine::ui::UITransform btn_trans;
    btn_trans.local_pos = {700.0f, 550.0f};
    btn_trans.size = {80.0f, 40.0f};
    GetRegistry().AddComponent(pause_button_, btn_trans);
    GetRegistry().AddComponent(
        pause_button_, engine::ecs::components::Quad{{0.5f, 0.5f, 0.5f, 1.0f}});
    GetRegistry().AddComponent(pause_button_, engine::ui::UIHierarchy{});

    engine::ui::UIInteractable btn_inter;
    btn_inter.on_click = [this]() { ToggleMenu(); };
    btn_inter.on_hover_changed = [this](bool hovered) {
      float start_scale = hovered ? 1.0f : 1.1f;
      float end_scale = hovered ? 1.1f : 1.0f;
      engine::util::TweenManager::Get()
          .Tween<float>(start_scale, end_scale, 0.1f)
          .OnUpdate([this](const float& val, float progress) {
            auto& t = GetRegistry().GetComponent<engine::ui::UITransform>(
                pause_button_);
            t.size = {80.0f * val, 40.0f * val};
          })
          .Play();
    };
    GetRegistry().AddComponent(pause_button_, btn_inter);

    // Button Text
    engine::ecs::EntityID btn_text = GetRegistry().CreateEntity();
    engine::ui::UITransform bt_trans;
    bt_trans.local_pos = {5.0f, 10.0f};
    GetRegistry().AddComponent(btn_text, bt_trans);
    GetRegistry().AddComponent(
        btn_text, engine::ecs::components::Text{
                      "PAUSE", "default", 0.8f, {1.0f, 1.0f, 1.0f, 1.0f}});
    engine::ui::UIHierarchy bt_hier;
    bt_hier.parent = pause_button_;
    GetRegistry().AddComponent(btn_text, bt_hier);
    GetRegistry()
        .GetComponent<engine::ui::UIHierarchy>(pause_button_)
        .children.push_back(btn_text);

    // 5. Menu Panel (Initially off-screen)
    menu_panel_ = GetRegistry().CreateEntity();
    engine::ui::UITransform menu_trans;
    menu_trans.local_pos = {800.0f, 150.0f};  // Off-screen right
    menu_trans.size = {300.0f, 300.0f};
    GetRegistry().AddComponent(menu_panel_, menu_trans);
    GetRegistry().AddComponent(
        menu_panel_, engine::ecs::components::Quad{{0.2f, 0.2f, 0.2f, 0.9f}});
    GetRegistry().AddComponent(menu_panel_, engine::ui::UIHierarchy{});

    engine::ecs::EntityID menu_text = GetRegistry().CreateEntity();
    engine::ui::UITransform mt_trans;
    mt_trans.local_pos = {50.0f, 250.0f};
    GetRegistry().AddComponent(menu_text, mt_trans);
    GetRegistry().AddComponent(
        menu_text, engine::ecs::components::Text{
                       "PAUSED", "default", 1.5f, {1.0f, 1.0f, 1.0f, 1.0f}});
    engine::ui::UIHierarchy mt_hier;
    mt_hier.parent = menu_panel_;
    GetRegistry().AddComponent(menu_text, mt_hier);
    GetRegistry()
        .GetComponent<engine::ui::UIHierarchy>(menu_panel_)
        .children.push_back(menu_text);
  }

  void OnUpdate(float delta_time_seconds) override {
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
        GetRegistry().GetComponent<engine::ui::UIInteractable>(pause_button_);
    auto& btn_quad = GetRegistry().GetComponent<engine::ecs::components::Quad>(
        pause_button_);
    if (btn_inter.is_hovered) {
      btn_quad.color = {0.7f, 0.7f, 0.7f, 1.0f};
    } else {
      btn_quad.color = {0.5f, 0.5f, 0.5f, 1.0f};
    }
  }

  void OnRender() override {
    engine::graphics::Renderer::Get().DrawQuad(player_pos_, {32.0f, 32.0f},
                                               {0.0f, 1.0f, 0.0f, 1.0f});
    engine::graphics::Renderer::Get().DrawQuad(
        "default", "Use WASD to move, Space to get gold", {10.0f, 10.0f}, 0.0f,
        0.7f, {1.0f, 1.0f, 1.0f, 1.0f});
    if (engine::ActionManager::Get().IsConsumed()) {
      engine::graphics::Renderer::Get().DrawQuad(
          "default", "INPUT CONSUMED BY UI", {300.0f, 550.0f}, 0.0f, 0.8f,
          {1.0f, 0.0f, 0.0f, 1.0f});
    }
    engine::graphics::Renderer::Get().Flush();
  }

  void ToggleMenu() {
    is_menu_open_ = !is_menu_open_;
    float target_x = is_menu_open_ ? 250.0f : 800.0f;
    auto& trans =
        GetRegistry().GetComponent<engine::ui::UITransform>(menu_panel_);
    float start_x = trans.local_pos.x;
    engine::util::TweenManager::Get()
        .Tween<float>(start_x, target_x, 0.5f)
        .Ease(engine::util::EaseType::OutCubic)
        .OnUpdate([this](const float& val, float progress) {
          auto& t =
              GetRegistry().GetComponent<engine::ui::UITransform>(menu_panel_);
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

class UiMenuApp : public demos::common::BaseDemoApp {
 public:
  void OnDemoInit() override {
    engine::SceneManager::Get().SetScene(
        std::make_unique<MenuScene>("UI Menu Demo"));
  }
};

int main(void) {
  engine::EngineConfig config;
  config.window_width = 800;
  config.window_height = 600;
  config.asset_path = ENGINE_ASSETS_PATH;
  return demos::common::DemoRunner::Run<UiMenuApp>(config);
}
