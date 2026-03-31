#include <cassert>
#include <iostream>

#include <engine/ecs/entity_manager.h>
#include <engine/ecs/registry.h>

struct TestComponent {
  int value;
};

void TestECS() {
  engine::ecs::Registry registry;

  // Test Entity Creation
  auto e1 = registry.CreateEntity();
  auto e2 = registry.CreateEntity();
  assert(registry.IsAlive(e1));
  assert(registry.IsAlive(e2));
  assert(e1 != e2);

  // Test Component Addition
  registry.AddComponent(e1, TestComponent{42});
  assert(registry.HasComponent<TestComponent>(e1));
  assert(registry.GetComponent<TestComponent>(e1).value == 42);

  // Test Component Patching
  registry.PatchComponent<TestComponent>(
      e1, [](TestComponent& c) { c.value = 100; });
  assert(registry.GetComponent<TestComponent>(e1).value == 100);

  // Test View
  int count = 0;
  auto view = registry.GetView<TestComponent>();
  for (auto entity : view) {
    count++;
  }
  assert(count == 1);

  // Test Entity Deletion
  registry.DeleteEntity(e1);
  assert(!registry.IsAlive(e1));
  assert(!registry.HasComponent<TestComponent>(e1));

  std::cout << "ECS Tests Passed!" << std::endl;
}

int main() {
  TestECS();
  return 0;
}
