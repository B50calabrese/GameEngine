#include "level_loader.h"

#include <fstream>
#include <iostream>
#include <sstream>

#include <glm/geometric.hpp>

#include <engine/ecs/components/graphics_components.h>
#include <engine/ecs/components/physics_components.h>
#include <engine/ecs/components/transform.h>

#include "components.h"

namespace platformer {

void LevelLoader::Load(const std::string& path,
                       engine::ecs::Registry& registry) {
  std::ifstream file(path);
  if (!file.is_open()) {
    std::cerr << "Failed to open level file: " << path << std::endl;
    return;
  }

  std::string line;
  while (std::getline(file, line)) {
    if (line.empty() || line[0] == '#') {
      continue;
    }

    std::stringstream ss(line);
    char type;
    ss >> type;

    switch (type) {
      case 'P': {
        float x, y, w, h;
        char p_type;
        ss >> x >> y >> w >> h >> p_type;

        auto entity = registry.CreateEntity();
        registry.AddComponent(
            entity, engine::ecs::components::Transform{{x, y}, {w, h}});
        registry.AddComponent(entity, engine::ecs::components::Collider{
                                          {w, h}, {0, 0}, true, false});

        PlatformComponent pc;
        switch (p_type) {
          case 'S':
            pc.type = PlatformType::Stationary;
            registry.AddComponent(entity, engine::ecs::components::Sprite{"textures/platform.png"});
            break;
          case 'M': {
            pc.type = PlatformType::Moving;
            float sx, sy, ex, ey;
            ss >> sx >> sy >> ex >> ey;
            pc.start_pos = {sx, sy};
            pc.end_pos = {ex, ey};
            registry.AddComponent(
                entity,
                engine::ecs::components::Velocity{
                    glm::normalize(pc.end_pos - pc.start_pos) * 150.0f});
            registry.AddComponent(entity, engine::ecs::components::Sprite{"textures/platform_moving.png"});
          } break;
          case 'T':
            pc.type = PlatformType::Temporary;
            registry.AddComponent(entity, engine::ecs::components::Sprite{"textures/platform_temp.png"});
            break;
        }
        registry.AddComponent(entity, pc);
      } break;

      case 'E': {
        float x, y;
        char e_type;
        ss >> x >> y;
        ss >> e_type;

        auto entity = registry.CreateEntity();
        registry.AddComponent(
            entity, engine::ecs::components::Transform{{x, y}, {64.0f, 64.0f}});
        registry.AddComponent(
            entity, engine::ecs::components::Sprite{"textures/robot_idle.png"});

        EnemyComponent ec;
        switch (e_type) {
          case 'S':
            ec.is_patrolling = false;
            break;
          case 'P': {
            ec.is_patrolling = true;
            float sx, sy, ex, ey;
            ss >> sx >> sy >> ex >> ey;
            ec.start_pos = {sx, sy};
            ec.end_pos = {ex, ey};
            registry.AddComponent(
                entity,
                engine::ecs::components::Velocity{
                    glm::normalize(ec.end_pos - ec.start_pos) * 100.0f});
          } break;
        }
        registry.AddComponent(entity, ec);
      } break;

      case 'G': {
        float x, y;
        ss >> x >> y;

        auto entity = registry.CreateEntity();
        registry.AddComponent(
            entity, engine::ecs::components::Transform{{x, y}, {64.0f, 64.0f}});
        registry.AddComponent(
            entity, engine::ecs::components::Sprite{"textures/door.png"});
        registry.AddComponent(entity, GoalComponent{});
      } break;
    }
  }
}

}  // namespace platformer
