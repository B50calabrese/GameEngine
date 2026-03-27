#include <string>

#include <engine/ecs/registry.h>
#include <engine/util/logger.h>

#include "../common/demo_utils.h"

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
    LOG_INFO("[EntityLogger] Entity Created: {}", event.entity);
  }

  void OnEvent(const EntityDestroyedEvent& event) override {
    LOG_INFO("[EntityLogger] Entity Destroyed: {}", event.entity);
  }

  void OnEvent(const ComponentAddedEvent<Position>& event) override {
    LOG_INFO("[EntityLogger] Position Added to Entity {}: ({:.2f}, {:.2f})",
             event.entity, event.component.x, event.component.y);
  }

  void OnEvent(const ComponentModifiedEvent<Position>& event) override {
    LOG_INFO("[EntityLogger] Position Modified on Entity {}: ({:.2f}, {:.2f})",
             event.entity, event.component.x, event.component.y);
  }

  void OnEvent(const ComponentRemovedEvent<Position>& event) override {
    LOG_INFO("[EntityLogger] Position Removed from Entity {}", event.entity);
  }

  void OnEvent(const GameEvent& event) override {
    LOG_INFO("[EntityLogger] Game Event Received: {}", event.message);
  }
};

class OneShotListener : public IEventListener<GameEvent> {
 public:
  void OnEvent(const GameEvent& event) override {
    LOG_INFO("[OneShotListener] Received: {}", event.message);
  }
};

class EcsNotificationsDemo : public demos::common::BaseDemoApp {
 public:
  void OnDemoInit() override {
    EntityLogger* logger = new EntityLogger(); // Keep it alive for simplicity in demo
    OneShotListener* one_shot = new OneShotListener();

    registry_.Subscribe<EntityCreatedEvent>(logger);
    registry_.Subscribe<EntityDestroyedEvent>(logger);
    registry_.Subscribe<ComponentAddedEvent<Position>>(logger);
    registry_.Subscribe<ComponentModifiedEvent<Position>>(logger);
    registry_.Subscribe<ComponentRemovedEvent<Position>>(logger);
    registry_.Subscribe<GameEvent>(logger);
    registry_.Subscribe<GameEvent>(one_shot, true);

    LOG_INFO("--- Testing Lifecycle Events ---");
    EntityID e1 = registry_.CreateEntity();
    registry_.AddComponent<Position>(e1, {10.0f, 20.0f});

    LOG_INFO("--- Testing Component Modification (Sync) ---");
    registry_.PatchComponent<Position>(e1, [](Position& p) {
      p.x += 5.0f;
      p.y += 10.0f;
    });

    LOG_INFO("--- Testing Generic Events (Sync) ---");
    registry_.Publish<GameEvent>({"Hello, World!"});
    registry_.Publish<GameEvent>({"This should only be seen by EntityLogger"});

    LOG_INFO("--- Testing Async Events ---");
    registry_.Publish<GameEvent>({"Queued Event 1"}, false);
    registry_.Publish<GameEvent>({"Queued Event 2"}, false);
    LOG_INFO("Events queued, calling Update()...");
    registry_.Update();

    LOG_INFO("--- Testing Component Removal ---");
    registry_.RemoveComponent<Position>(e1);

    LOG_INFO(
        "--- Testing Entity Destruction (with implicit component removal) ---");
    EntityID e2 = registry_.CreateEntity();
    registry_.AddComponent<Position>(e2, {100.0f, 200.0f});
    registry_.DeleteEntity(e2);

    LOG_INFO("--- Testing Unsubscribe ---");
    registry_.Unsubscribe<GameEvent>(logger);
    registry_.Publish<GameEvent>({"This should not be logged"});
  }

  void OnDemoUpdate(double dt) override {
    // Already shut down the demo logic in Init for this specific non-interactive demo
    engine::Engine::Shutdown();
  }

 private:
  Registry registry_;
};

int main() {
  engine::EngineConfig config;
  config.window_title = "ECS Notifications Demo";
  return demos::common::DemoRunner::Run<EcsNotificationsDemo>(config);
}
