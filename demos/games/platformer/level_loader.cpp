#include <fstream>
#include <iostream>
#include <sstream>

#include <glm/geometric.hpp>

#include "components.h"
#include "level_loader.h"

namespace platformer {

void LevelLoader::Load(const std::string& path, engine::ecs::Registry& registry) {
  std::ifstream file(path);
  if (!file.is_open()) {
    std::cerr << "Failed to open level file: " << path << std::endl;
    return;
  }

  std::string line;
  while (std::getline(file, line)) {
    if (line.empty() || line[0] == '#') continue;

    std::stringstream ss(line);
    char type;
    ss >> type;

    if (type == 'P') {
      float x, y, w, h;
      char p_type;
      ss >> x >> y >> w >> h >> p_type;

      auto entity = registry.CreateEntity();
      PlatformComponent pc;
      pc.position = {x, y};
      pc.size = {w, h};
      if (p_type == 'S') {
        pc.type = PlatformType::Stationary;
      } else if (p_type == 'M') {
        pc.type = PlatformType::Moving;
        float sx, sy, ex, ey;
        ss >> sx >> sy >> ex >> ey;
        pc.start_pos = {sx, sy};
        pc.end_pos = {ex, ey};
        pc.velocity = glm::normalize(pc.end_pos - pc.start_pos) * 150.0f;
      } else if (p_type == 'T') {
        pc.type = PlatformType::Temporary;
      }
      registry.AddComponent(entity, pc);

    } else if (type == 'E') {
      float x, y;
      char e_type;
      ss >> x >> y >> e_type;

      auto entity = registry.CreateEntity();
      EnemyComponent ec;
      ec.position = {x, y};
      if (e_type == 'S') {
        ec.is_patrolling = false;
      } else if (e_type == 'P') {
        ec.is_patrolling = true;
        float sx, sy, ex, ey;
        ss >> sx >> sy >> ex >> ey;
        ec.start_pos = {sx, sy};
        ec.end_pos = {ex, ey};
        ec.velocity = glm::normalize(ec.end_pos - ec.start_pos) * 100.0f;
      }
      registry.AddComponent(entity, ec);

    } else if (type == 'G') {
      float x, y;
      ss >> x >> y;

      auto entity = registry.CreateEntity();
      GoalComponent gc;
      gc.position = {x, y};
      registry.AddComponent(entity, gc);
    }
  }
}

}  // namespace platformer
