#include <string>

#include <engine/ecs/registry.h>
#include <engine/util/logger.h>

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
    LOG_INFO("[EntityLogger] Entity Created: %u", event.entity);
  }

  void OnEvent(const EntityDestroyedEvent& event) override {
    LOG_INFO("[EntityLogger] Entity Destroyed: %u", event.entity);
  }

  void OnEvent(const ComponentAddedEvent<Position>& event) override {
    LOG_INFO("[EntityLogger] Position Added to Entity %u: (%.2f, %.2f)",
             event.entity, event.component.x, event.component.y);
  }

  void OnEvent(const ComponentModifiedEvent<Position>& event) override {
    LOG_INFO("[EntityLogger] Position Modified on Entity %u: (%.2f, %.2f)",
             event.entity, event.component.x, event.component.y);
  }

  void OnEvent(const ComponentRemovedEvent<Position>& event) override {
    LOG_INFO("[EntityLogger] Position Removed from Entity %u", event.entity);
  }

  void OnEvent(const GameEvent& event) override {
    LOG_INFO("[EntityLogger] Game Event Received: %s", event.message.c_str());
  }
};

class OneShotListener : public IEventListener<GameEvent> {
 public:
  void OnEvent(const GameEvent& event) override {
    LOG_INFO("[OneShotListener] Received: %s", event.message.c_str());
  }
};

int main() {
  // Initialize logger for the demo
  engine::util::Logger::Get().Init();

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

  LOG_INFO("--- Testing Lifecycle Events ---");
  EntityID e1 = registry.CreateEntity();
  registry.AddComponent<Position>(e1, {10.0f, 20.0f});

  LOG_INFO("--- Testing Component Modification (Sync) ---");
  registry.PatchComponent<Position>(e1, [](Position& p) {
    p.x += 5.0f;
    p.y += 10.0f;
  });

  LOG_INFO("--- Testing Generic Events (Sync) ---");
  registry.Publish<GameEvent>({"Hello, World!"});
  registry.Publish<GameEvent>({"This should only be seen by EntityLogger"});

  LOG_INFO("--- Testing Async Events ---");
  registry.Publish<GameEvent>({"Queued Event 1"}, false);
  registry.Publish<GameEvent>({"Queued Event 2"}, false);
  LOG_INFO("Events queued, calling Update()...");
  registry.Update();

  LOG_INFO("--- Testing Component Removal ---");
  registry.RemoveComponent<Position>(e1);

  LOG_INFO(
      "--- Testing Entity Destruction (with implicit component removal) ---");
  EntityID e2 = registry.CreateEntity();
  registry.AddComponent<Position>(e2, {100.0f, 200.0f});
  registry.DeleteEntity(e2);

  LOG_INFO("--- Testing Unsubscribe ---");
  registry.Unsubscribe<GameEvent>(&logger);
  registry.Publish<GameEvent>({"This should not be logged"});

  return 0;
}
