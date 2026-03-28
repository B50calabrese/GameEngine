#ifndef DEMOS_GAMES_TACTICAL_RPG_MAP_SCENE_H_
#define DEMOS_GAMES_TACTICAL_RPG_MAP_SCENE_H_

#include <algorithm>
#include <memory>
#include <vector>

#include <engine/graphics/ecs/text_renderer.h>
#include <engine/graphics/renderer.h>
#include <engine/input/input_manager.h>
#include <engine/scene/scene.h>
#include <engine/scene/scene_manager.h>

#include "map_node.h"
#include "map_system.h"

namespace tactical_rpg {

class MapScene : public engine::Scene {
 public:
  MapScene(const std::string& name,
           const std::vector<std::shared_ptr<MapNode>>& map,
           int current_node_id)
      : engine::Scene(name), map_(map), current_node_id_(current_node_id) {
    selected_node_index_ = -1;
  }

  void OnAttach() override {
    next_node_ids_.clear();
    for (int conn : map_[current_node_id_]->GetConnections()) {
      next_node_ids_.push_back(conn);
    }
    if (!next_node_ids_.empty()) selected_node_index_ = 0;
  }

  void OnUpdate(float dt) override {
    auto& input = engine::InputManager::Get();
    if (next_node_ids_.empty()) return;

    if (input.IsKeyPressed(engine::KeyCode::kA))
      selected_node_index_ =
          (selected_node_index_ - 1 + next_node_ids_.size()) %
          next_node_ids_.size();
    if (input.IsKeyPressed(engine::KeyCode::kD))
      selected_node_index_ = (selected_node_index_ + 1) % next_node_ids_.size();

    if (input.IsKeyPressed(engine::KeyCode::kSpace)) {
      int next_id = next_node_ids_[selected_node_index_];
      EnterNode(next_id);
    }
  }

  void EnterNode(int node_id);

  void OnRender() override {
    engine::graphics::ecs::TextRenderer::Get().DrawText(
        "default", "MAP SCREEN", {100, 650}, 0.0f, 1.0f, {1, 1, 1, 1});

    for (const auto& node : map_) {
      for (int conn_id : node->GetConnections()) {
        const auto& target = map_[conn_id];
        glm::vec2 start = node->GetPosition();
        glm::vec2 end = target->GetPosition();
        glm::vec2 diff = end - start;
        engine::graphics::Renderer::Get().DrawQuad(start + diff * 0.5f,
                                                   {glm::length(diff), 2.0f},
                                                   {0.5f, 0.5f, 0.5f, 1.0f});
      }
    }

    for (const auto& node : map_) {
      glm::vec4 color = {0.3f, 0.3f, 0.3f, 1.0f};
      if (node->GetId() == current_node_id_)
        color = {1, 1, 0, 1};
      else {
        auto it = std::find(next_node_ids_.begin(), next_node_ids_.end(),
                            node->GetId());
        if (it != next_node_ids_.end()) {
          if (selected_node_index_ != -1 &&
              next_node_ids_[selected_node_index_] == node->GetId())
            color = {0, 1, 1, 1};
          else
            color = {0, 1, 0, 1};
        }
      }

      std::string label = "?";
      switch (node->GetType()) {
        case NodeType::Fight:
          label = "F";
          break;
        case NodeType::Shop:
          label = "S";
          break;
        case NodeType::Rest:
          label = "R";
          break;
        case NodeType::Event:
          label = "E";
          break;
        case NodeType::Boss:
          label = "B";
          break;
        case NodeType::Start:
          label = "O";
          break;
      }

      engine::graphics::Renderer::Get().DrawQuad(node->GetPosition(), {30, 30},
                                                 color);
      engine::graphics::ecs::TextRenderer::Get().DrawText(
          "default", label, node->GetPosition() - glm::vec2(5, 5), 0.0f, 0.5f,
          {1, 1, 1, 1});
    }

    engine::graphics::Renderer::Get().Flush();
  }

 private:
  const std::vector<std::shared_ptr<MapNode>>& map_;
  int current_node_id_;
  std::vector<int> next_node_ids_;
  int selected_node_index_ = -1;
};

}  // namespace tactical_rpg

#endif  // DEMOS_GAMES_TACTICAL_RPG_MAP_SCENE_H_
