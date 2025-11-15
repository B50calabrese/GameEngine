#include "engine.h"

#include <iostream>

namespace engine {

Engine::Engine() = default;

Engine::~Engine() = default;

void Engine::Init() { std::cout << "Success\n"; }

}  // namespace engine