#ifndef DEMOS_GAMES_TACTICAL_RPG_MAP_SYSTEM_H_
#define DEMOS_GAMES_TACTICAL_RPG_MAP_SYSTEM_H_

#include <algorithm>
#include <memory>
#include <random>
#include <vector>

#include <glm/glm.hpp>

#include "map_node.h"

namespace tactical_rpg {

class MapSystem {
 public:
  static std::vector<std::shared_ptr<MapNode>> GenerateMap(int floors,
                                                           int nodes_per_floor,
                                                           uint32_t seed) {
    std::vector<std::shared_ptr<MapNode>> map;
    std::mt19937 gen(seed);
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);

    int id_counter = 0;

    // Start node
    auto start = std::make_shared<StartNode>(id_counter++, NodeType::Start,
                                             glm::vec2{100.0f, 360.0f}, 0);
    map.push_back(start);

    std::vector<int> last_floor_ids = {start->GetId()};

    for (int f = 1; f <= floors; ++f) {
      std::vector<int> current_floor_ids;
      for (int n = 0; n < nodes_per_floor; ++n) {
        int id = id_counter++;
        NodeType type;

        float r = dist(gen);
        if (f == floors) {
          type = NodeType::Boss;
        } else if (r < 0.6f) {
          type = NodeType::Fight;
        } else if (r < 0.75f) {
          type = NodeType::Event;
        } else if (r < 0.9f) {
          type = NodeType::Shop;
        } else {
          type = NodeType::Rest;
        }

        float y_spacing = 600.0f / (nodes_per_floor + 1);
        glm::vec2 pos = {100.0f + f * 100.0f, (n + 1) * y_spacing};

        std::shared_ptr<MapNode> node;
        if (type == NodeType::Fight || type == NodeType::Boss) {
          node = std::make_shared<FightNode>(id, type, pos, f);
        } else if (type == NodeType::Rest) {
          node = std::make_shared<RestNode>(id, type, pos, f);
        } else {
          node = std::make_shared<PlaceholderNode>(id, type, pos, f);
        }

        map.push_back(node);
        current_floor_ids.push_back(id);
      }

      for (int last_id : last_floor_ids) {
        int num_conns = (dist(gen) < 0.3f) ? 2 : 1;
        std::vector<int> targets = current_floor_ids;
        std::shuffle(targets.begin(), targets.end(), gen);
        for (int i = 0; i < std::min(num_conns, (int)targets.size()); ++i) {
          map[last_id]->AddConnection(targets[i]);
        }
      }

      for (int curr_id : current_floor_ids) {
        bool reachable = false;
        for (int last_id : last_floor_ids) {
          for (int conn : map[last_id]->GetConnections()) {
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
          map[random_last]->AddConnection(curr_id);
        }
      }

      last_floor_ids = current_floor_ids;
    }

    return map;
  }
};

}  // namespace tactical_rpg

#endif  // DEMOS_GAMES_TACTICAL_RPG_MAP_SYSTEM_H_
