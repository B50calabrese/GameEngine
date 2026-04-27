#include <gtest/gtest.h>
#include <engine/ecs/registry.h>

namespace engine::ecs {

struct Position {
  float x, y;
};

struct Velocity {
  float vx, vy;
};

class RegistryTest : public ::testing::Test {
 protected:
  Registry registry;
};

TEST_F(RegistryTest, EntityLifecycle) {
  EntityID e = registry.CreateEntity();
  EXPECT_TRUE(registry.IsAlive(e));

  registry.DeleteEntity(e);
  EXPECT_FALSE(registry.IsAlive(e));
}

TEST_F(RegistryTest, ComponentManagement) {
  EntityID e = registry.CreateEntity();

  registry.AddComponent<Position>(e, {10.0f, 20.0f});
  EXPECT_TRUE(registry.HasComponent<Position>(e));

  Position& pos = registry.GetComponent<Position>(e);
  EXPECT_EQ(pos.x, 10.0f);
  EXPECT_EQ(pos.y, 20.0f);

  registry.RemoveComponent<Position>(e);
  EXPECT_FALSE(registry.HasComponent<Position>(e));
}

TEST_F(RegistryTest, ViewIteration) {
  EntityID e1 = registry.CreateEntity();
  EntityID e2 = registry.CreateEntity();
  EntityID e3 = registry.CreateEntity();

  registry.AddComponent<Position>(e1, {1.0f, 1.0f});
  registry.AddComponent<Velocity>(e1, {1.0f, 1.0f});

  registry.AddComponent<Position>(e2, {2.0f, 2.0f});

  registry.AddComponent<Position>(e3, {3.0f, 3.0f});
  registry.AddComponent<Velocity>(e3, {3.0f, 3.0f});

  int count = 0;
  registry.ForEach<Position, Velocity>([&count](Position& p, Velocity& v) {
    count++;
  });

  EXPECT_EQ(count, 2);
}

TEST_F(RegistryTest, PatchComponent) {
  EntityID e = registry.CreateEntity();
  registry.AddComponent<Position>(e, {10.0f, 10.0f});

  registry.PatchComponent<Position>(e, [](Position& p) {
    p.x += 5.0f;
  });

  EXPECT_EQ(registry.GetComponent<Position>(e).x, 15.0f);
}

TEST_F(RegistryTest, Clear) {
  EntityID e = registry.CreateEntity();
  registry.AddComponent<Position>(e, {1.0f, 1.0f});

  registry.Clear();
  EXPECT_FALSE(registry.IsAlive(e));

  EntityID new_e = registry.CreateEntity();
  EXPECT_EQ(new_e, 0); // Assuming ID reuse from 0 after clear
}

} // namespace engine::ecs
