#ifndef DEMOS_GAMES_TACTICAL_RPG_MAP_NODE_H_
#define DEMOS_GAMES_TACTICAL_RPG_MAP_NODE_H_

#include <string>
#include <vector>

#include <glm/glm.hpp>

namespace tactical_rpg {

enum class NodeType { Rest, Event, Shop, Fight, Boss, Start };

class MapNode {
 public:
  MapNode(int id, NodeType type, glm::vec2 pos, int floor)
      : id_(id), type_(type), position_(pos), floor_(floor) {}
  virtual ~MapNode() = default;

  virtual void OnEnter() = 0;
  virtual void OnExit() = 0;

  int GetId() const { return id_; }
  NodeType GetType() const { return type_; }
  glm::vec2 GetPosition() const { return position_; }
  int GetFloor() const { return floor_; }
  const std::vector<int>& GetConnections() const { return connections_; }
  void AddConnection(int target_id) { connections_.push_back(target_id); }

 protected:
  int id_;
  NodeType type_;
  glm::vec2 position_;
  int floor_;
  std::vector<int> connections_;
};

class FightNode : public MapNode {
 public:
  using MapNode::MapNode;
  void OnEnter() override;
  void OnExit() override {}
};

class RestNode : public MapNode {
 public:
  using MapNode::MapNode;
  void OnEnter() override;
  void OnExit() override {}
};

class StartNode : public MapNode {
 public:
  using MapNode::MapNode;
  void OnEnter() override {}
  void OnExit() override {}
};

class PlaceholderNode : public MapNode {
 public:
  using MapNode::MapNode;
  void OnEnter() override;
  void OnExit() override {}
};

}  // namespace tactical_rpg

#endif  // DEMOS_GAMES_TACTICAL_RPG_MAP_NODE_H_
