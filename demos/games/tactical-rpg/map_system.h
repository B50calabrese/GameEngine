#ifndef DEMOS_GAMES_TACTICAL_RPG_MAP_SYSTEM_H_
#define DEMOS_GAMES_TACTICAL_RPG_MAP_SYSTEM_H_

#include <algorithm>
#include <memory>
#include <random>
#include <vector>

#include <glm/glm.hpp>

namespace tactical_rpg {

enum class NodeType { Rest, Event, Shop, Fight, Boss, Start };

struct MapNode {
  int id;
  NodeType type;
  glm::vec2 position;            // For rendering
  std::vector<int> connections;  // Indices of next nodes
  int floor;
  bool is_visited = false;
};

class MapSystem {
 public:
  static std::vector<MapNode> GenerateMap(int floors, int nodes_per_floor,
                                          uint32_t seed) {
    std::vector<MapNode> map;
    std::mt19937 gen(seed);
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);

    int id_counter = 0;

    // Start node
    MapNode start;
    start.id = id_counter++;
    start.type = NodeType::Start;
    start.floor = 0;
    start.position = {400.0f, 550.0f};  // Bottom center
    map.push_back(start);

    std::vector<int> last_floor_ids = {start.id};

    for (int f = 1; f <= floors; ++f) {
      std::vector<int> current_floor_ids;
      for (int n = 0; n < nodes_per_floor; ++n) {
        MapNode node;
        node.id = id_counter++;
        node.floor = f;

        // Random type
        float r = dist(gen);
        if (f == floors) {
          node.type = NodeType::Boss;
        } else if (r < 0.6f) {
          node.type = NodeType::Fight;
        } else if (r < 0.75f) {
          node.type = NodeType::Event;
        } else if (r < 0.9f) {
          node.type = NodeType::Shop;
        } else {
          node.type = NodeType::Rest;
        }

        // Position
        float x_spacing = 800.0f / (nodes_per_floor + 1);
        node.position = {(n + 1) * x_spacing, 550.0f - f * 100.0f};

        // Connect from last floor
        // Simplification: each node in last floor connects to at least one in
        // current, and each node in current floor has at least one connection
        // from last. For STS style, we'll just do some random connections to
        // neighbors.

        map.push_back(node);
        current_floor_ids.push_back(node.id);
      }

      // Create connections from last floor to current floor
      for (int last_id : last_floor_ids) {
        // Each node from previous floor connects to 1-2 nodes in current floor
        int num_conns = (dist(gen) < 0.3f) ? 2 : 1;
        std::vector<int> targets = current_floor_ids;
        std::shuffle(targets.begin(), targets.end(), gen);
        for (int i = 0; i < std::min(num_conns, (int)targets.size()); ++i) {
          map[last_id].connections.push_back(targets[i]);
        }
      }

      // Ensure every current node is reachable
      for (int curr_id : current_floor_ids) {
        bool reachable = false;
        for (int last_id : last_floor_ids) {
          for (int conn : map[last_id].connections) {
            if (conn == curr_id) {
              reachable = true;
              break;
            }
          }
          if (reachable) break;
        }
        if (!reachable) {
          int random_last = last_floor_ids[std::uniform_int_distribution<int>(
              0, last_floor_ids.size() - 1)(gen)];
          map[random_last].connections.push_back(curr_id);
        }
      }

      last_floor_ids = current_floor_ids;
    }

    return map;
  }
};

}  // namespace tactical_rpg

#endif  // DEMOS_GAMES_TACTICAL_RPG_MAP_SYSTEM_H_
