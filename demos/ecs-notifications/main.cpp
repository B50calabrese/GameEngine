#include <iostream>
#include <string>

#include <engine/ecs/registry.h>

using namespace engine::ecs;

struct Position {
  float x, y;
};

struct Name {
  std::string value;
};

// Generic event
struct GameEvent {
  std::string message;
};

class EntityLogger : public IEventListener<EntityCreatedEvent>,
                     public IEventListener<EntityDestroyedEvent>,
                     public IEventListener<ComponentAddedEvent<Position>>,
                     public IEventListener<ComponentModifiedEvent<Position>>,
                     public IEventListener<ComponentRemovedEvent<Position>>,
                     public IEventListener<GameEvent> {
 public:
  void OnEvent(const EntityCreatedEvent& event) override {
    std::cout << "[EntityLogger] Entity Created: " << event.entity << std::endl;
  }

  void OnEvent(const EntityDestroyedEvent& event) override {
    std::cout << "[EntityLogger] Entity Destroyed: " << event.entity
              << std::endl;
  }

  void OnEvent(const ComponentAddedEvent<Position>& event) override {
    std::cout << "[EntityLogger] Position Added to Entity " << event.entity
              << ": (" << event.component.x << ", " << event.component.y << ")"
              << std::endl;
  }

  void OnEvent(const ComponentModifiedEvent<Position>& event) override {
    std::cout << "[EntityLogger] Position Modified on Entity " << event.entity
              << ": (" << event.component.x << ", " << event.component.y << ")"
              << std::endl;
  }

  void OnEvent(const ComponentRemovedEvent<Position>& event) override {
    std::cout << "[EntityLogger] Position Removed from Entity " << event.entity
              << std::endl;
  }

  void OnEvent(const GameEvent& event) override {
    std::cout << "[EntityLogger] Game Event Received: " << event.message
              << std::endl;
  }
};

class OneShotListener : public IEventListener<GameEvent> {
 public:
  void OnEvent(const GameEvent& event) override {
    std::cout << "[OneShotListener] Received: " << event.message << std::endl;
  }
};

int main() {
  Registry registry;
  EntityLogger logger;
  OneShotListener one_shot;

  registry.Subscribe<EntityCreatedEvent>(&logger);
  registry.Subscribe<EntityDestroyedEvent>(&logger);
  registry.Subscribe<ComponentAddedEvent<Position>>(&logger);
  registry.Subscribe<ComponentModifiedEvent<Position>>(&logger);
  registry.Subscribe<ComponentRemovedEvent<Position>>(&logger);
  registry.Subscribe<GameEvent>(&logger);
  registry.Subscribe<GameEvent>(&one_shot, true);

  std::cout << "--- Testing Lifecycle Events ---" << std::endl;
  EntityID e1 = registry.CreateEntity();
  registry.AddComponent<Position>(e1, {10.0f, 20.0f});

  std::cout << "\n--- Testing Component Modification (Sync) ---" << std::endl;
  registry.PatchComponent<Position>(e1, [](Position& p) {
    p.x += 5.0f;
    p.y += 10.0f;
  });

  std::cout << "\n--- Testing Generic Events (Sync) ---" << std::endl;
  registry.Publish<GameEvent>({"Hello, World!"});
  registry.Publish<GameEvent>({"This should only be seen by EntityLogger"});

  std::cout << "\n--- Testing Async Events ---" << std::endl;
  registry.Publish<GameEvent>({"Queued Event 1"}, false);
  registry.Publish<GameEvent>({"Queued Event 2"}, false);
  std::cout << "Events queued, calling Update()..." << std::endl;
  registry.Update();

  std::cout << "\n--- Testing Component Removal ---" << std::endl;
  registry.RemoveComponent<Position>(e1);

  std::cout << "\n--- Testing Entity Destruction (with implicit component "
               "removal) ---"
            << std::endl;
  EntityID e2 = registry.CreateEntity();
  registry.AddComponent<Position>(e2, {100.0f, 200.0f});
  registry.DeleteEntity(e2);

  std::cout << "\n--- Testing Unsubscribe ---" << std::endl;
  registry.Unsubscribe<GameEvent>(&logger);
  registry.Publish<GameEvent>({"This should not be logged"});

  return 0;
}
