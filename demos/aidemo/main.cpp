#include <memory>
#include <random>
#include <vector>

#include <glm/glm.hpp>

#include <engine/ecs/components/ai_components.h>
#include <engine/ecs/components/transform.h>
#include <engine/ecs/registry.h>
#include <engine/ecs/systems/ai_system.h>
#include <engine/graphics/renderer.h>
#include <engine/input/input_manager.h>
#include <engine/scene/scene.h>
#include <engine/scene/scene_manager.h>
#include <engine/util/behavior_tree.h>
#include <engine/util/fsm.h>
#include <engine/util/logger.h>

#include "../common/demo_utils.h"

// --- Components ---
struct NPCComponent {
  float health = 100.0f;
  float speed = 100.0f;
  glm::vec2 target_pos = {400, 300};
  std::string status_text = "Idle";
};

// --- Demo engine::Scene ---
class AIDemoScene : public engine::Scene {
 public:
  AIDemoScene() : engine::Scene("AIDemo") {}

  void OnAttach() override {
    // Create NPC entity
    npc_ = registry_.CreateEntity();
    registry_.AddComponent<engine::ecs::components::Transform>(
        npc_, {{100, 100}, {1, 1}, 0});
    registry_.AddComponent<NPCComponent>(npc_, NPCComponent());
    registry_.AddComponent<engine::ecs::components::StateMachine>(
        npc_, engine::ecs::components::StateMachine());
    registry_.AddComponent<engine::ecs::components::BehaviorTree>(
        npc_, engine::ecs::components::BehaviorTree());

    SetupFSM(
        registry_.GetComponent<engine::ecs::components::StateMachine>(npc_).fsm,
        npc_);
    SetupBT(registry_.GetComponent<engine::ecs::components::BehaviorTree>(npc_)
                .tree,
            npc_);

    registry_.GetComponent<engine::ecs::components::StateMachine>(npc_)
        .fsm.ChangeState("Patrol");
  }

  void SetupFSM(engine::util::StateMachine& fsm, engine::ecs::EntityID entity) {
    // Patrol State
    auto patrol = std::make_shared<engine::util::LambdaState>(
        [this, entity](float dt) {
          auto& transform =
              registry_.GetComponent<engine::ecs::components::Transform>(
                  entity);
          auto& npc = registry_.GetComponent<NPCComponent>(entity);
          npc.status_text = "State: Patrol (Using BT)";

          // In Patrol state, we let the BT handle movement.
          // But we check for "combat" trigger (e.g., proximity to mouse)
          glm::vec2 mouse_pos = engine::InputManager::Get().mouse_screen_pos();
          if (glm::distance(transform.position, mouse_pos) < 100.0f) {
            registry_
                .GetComponent<engine::ecs::components::StateMachine>(entity)
                .fsm.ChangeState("Combat");
          }
        },
        [entity, this]() {
          registry_.GetComponent<NPCComponent>(entity).speed = 100.0f;
        });

    // Combat State
    auto combat = std::make_shared<engine::util::LambdaState>(
        [this, entity](float dt) {
          auto& transform =
              registry_.GetComponent<engine::ecs::components::Transform>(
                  entity);
          auto& npc = registry_.GetComponent<NPCComponent>(entity);
          npc.status_text = "State: Combat (Aggressive)";

          glm::vec2 mouse_pos = engine::InputManager::Get().mouse_screen_pos();
          glm::vec2 dir = glm::normalize(mouse_pos - transform.position);
          transform.position += dir * npc.speed * 1.5f * dt;

          if (glm::distance(transform.position, mouse_pos) > 200.0f) {
            registry_
                .GetComponent<engine::ecs::components::StateMachine>(entity)
                .fsm.ChangeState("Patrol");
          }

          // Simulate health loss in combat
          npc.health -= 5.0f * dt;
          if (npc.health < 20.0f) {
            registry_
                .GetComponent<engine::ecs::components::StateMachine>(entity)
                .fsm.ChangeState("Flee");
          }
        },
        [entity, this]() {
          registry_.GetComponent<NPCComponent>(entity).speed = 150.0f;
        });

    // Flee State
    auto flee = std::make_shared<engine::util::LambdaState>([this,
                                                             entity](float dt) {
      auto& transform =
          registry_.GetComponent<engine::ecs::components::Transform>(entity);
      auto& npc = registry_.GetComponent<NPCComponent>(entity);
      npc.status_text = "State: Flee (Low Health!)";

      glm::vec2 center = {400, 300};
      glm::vec2 dir = glm::normalize(transform.position - center);
      if (glm::length(dir) < 0.01f) dir = {1, 0};
      transform.position += dir * npc.speed * 2.0f * dt;

      npc.health += 10.0f * dt;  // Regenerate while fleeing
      if (npc.health >= 100.0f) {
        npc.health = 100.0f;
        registry_.GetComponent<engine::ecs::components::StateMachine>(entity)
            .fsm.ChangeState("Patrol");
      }
    });

    fsm.AddState("Patrol", patrol);
    fsm.AddState("Combat", combat);
    fsm.AddState("Flee", flee);
  }

  void SetupBT(engine::util::BehaviorTree& tree, engine::ecs::EntityID entity) {
    auto root = std::make_shared<engine::util::SelectorNode>();

    // Condition: Is in Patrol state?
    auto is_patrolling = std::make_shared<engine::util::ConditionNode>(
        [this, entity](engine::util::Blackboard& bb) {
          auto& fsm =
              registry_
                  .GetComponent<engine::ecs::components::StateMachine>(entity)
                  .fsm;
          return fsm.GetCurrentStateName() == "Patrol";
        });

    // Sequence: Move to target
    auto move_sequence = std::make_shared<engine::util::SequenceNode>();

    auto find_target = std::make_shared<engine::util::ActionNode>(
        [this, entity](float dt, engine::util::Blackboard& bb) {
          if (!bb.Has("target")) {
            float rx = static_cast<float>(rand() % 600 + 100);
            float ry = static_cast<float>(rand() % 400 + 100);
            bb.Set<glm::vec2>("target", {rx, ry});
            return engine::util::NodeStatus::SUCCESS;
          }
          return engine::util::NodeStatus::SUCCESS;
        });

    auto move_to_target = std::make_shared<engine::util::ActionNode>(
        [this, entity](float dt, engine::util::Blackboard& bb) {
          auto& transform =
              registry_.GetComponent<engine::ecs::components::Transform>(
                  entity);
          auto& npc = registry_.GetComponent<NPCComponent>(entity);
          glm::vec2 target = bb.Get<glm::vec2>("target");

          if (glm::distance(transform.position, target) < 5.0f) {
            bb.Remove("target");
            return engine::util::NodeStatus::SUCCESS;
          }

          glm::vec2 dir = glm::normalize(target - transform.position);
          transform.position += dir * npc.speed * dt;
          return engine::util::NodeStatus::RUNNING;
        });

    move_sequence->AddChild(find_target);
    move_sequence->AddChild(move_to_target);

    // Only move if patrolling
    auto patrol_logic = std::make_shared<engine::util::SequenceNode>();
    patrol_logic->AddChild(is_patrolling);
    patrol_logic->AddChild(move_sequence);

    root->AddChild(patrol_logic);
    tree.SetRoot(root);
  }

  void OnUpdate(float dt) override {
    engine::ecs::AISystem::Update(&registry_, dt);
  }

  void OnRender() override {
    auto& transform =
        registry_.GetComponent<engine::ecs::components::Transform>(npc_);
    auto& npc = registry_.GetComponent<NPCComponent>(npc_);

    // Draw NPC
    glm::vec4 color = {0, 1, 0, 1};  // Patrol
    auto& fsm =
        registry_.GetComponent<engine::ecs::components::StateMachine>(npc_).fsm;
    if (fsm.GetCurrentStateName() == "Combat")
      color = {1, 0, 0, 1};
    else if (fsm.GetCurrentStateName() == "Flee")
      color = {1, 1, 0, 1};

    engine::graphics::Renderer::Get().DrawQuad(transform.position, {30, 30},
                                               color);

    // Draw UI
    engine::graphics::Renderer::Get().DrawText(
        "default", npc.status_text, {10, 570}, 0.0f, 0.7f, {1, 1, 1, 1});
    engine::graphics::Renderer::Get().DrawText(
        "default", "Health: " + std::to_string((int)npc.health), {10, 540},
        0.0f, 0.7f, {1, 1, 1, 1});
    engine::graphics::Renderer::Get().DrawText(
        "default", "Move mouse close to NPC to trigger Combat", {10, 20}, 0.0f,
        0.5f, {0.7f, 0.7f, 0.7f, 1.0f});

    engine::graphics::Renderer::Get().Flush();
  }

 private:
  engine::ecs::Registry registry_;
  engine::ecs::EntityID npc_;
};

class AIDemoApp : public demos::common::BaseDemoApp {
 public:
  void OnDemoInit() override {
    engine::SceneManager::Get().SetScene(std::make_unique<AIDemoScene>());
  }
};

int main() {
  engine::EngineConfig config;
  config.window_width = 800;
  config.window_height = 600;
  config.asset_path = ENGINE_ASSETS_PATH;
  return demos::common::DemoRunner::Run<AIDemoApp>(config);
}
