#ifndef DEMOS_GAMES_PLATFORMER_LEVEL_LOADER_H_
#define DEMOS_GAMES_PLATFORMER_LEVEL_LOADER_H_

#include <string>

#include <engine/ecs/registry.h>

namespace platformer {

class LevelLoader {
 public:
  static void Load(const std::string& path, engine::ecs::Registry& registry);
};

}  // namespace platformer

#endif  // DEMOS_GAMES_PLATFORMER_LEVEL_LOADER_H_
