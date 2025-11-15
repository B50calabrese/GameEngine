#ifndef GAME_ENGINE_INCLUDE_ENGINE_H_
#define GAME_ENGINE_INCLUDE_ENGINE_H_

namespace engine {

// A class that represents the game engine.
class Engine {
 public:
  // Default constructor.
  Engine();

  // Default destructor.
  ~Engine();

  // Initializes the game engine.
  static void Init();
};

}  // namespace engine

#endif  // GAME_ENGINE_INCLUDE_ENGINE_H_