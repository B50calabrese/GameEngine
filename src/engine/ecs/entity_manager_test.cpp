#include <gtest/gtest.h>
#include <engine/ecs/entity_manager.h>

namespace engine::ecs {

TEST(EntityManagerTest, CreateEntity) {
  EntityManager manager;
  EntityID e1 = manager.CreateEntity();
  EntityID e2 = manager.CreateEntity();
  EXPECT_NE(e1, e2);
  EXPECT_TRUE(manager.IsAlive(e1));
  EXPECT_TRUE(manager.IsAlive(e2));
}

TEST(EntityManagerTest, DestroyEntity) {
  EntityManager manager;
  EntityID e1 = manager.CreateEntity();
  EXPECT_TRUE(manager.IsAlive(e1));
  manager.DestroyEntity(e1);
  EXPECT_FALSE(manager.IsAlive(e1));
}

TEST(EntityManagerTest, ReuseEntityID) {
  EntityManager manager;
  EntityID e1 = manager.CreateEntity();
  manager.DestroyEntity(e1);
  EntityID e2 = manager.CreateEntity();
  EXPECT_EQ(e1, e2);
  EXPECT_TRUE(manager.IsAlive(e2));
}

TEST(EntityManagerTest, Clear) {
  EntityManager manager;
  manager.CreateEntity();
  manager.CreateEntity();
  manager.Clear();
  EXPECT_EQ(manager.next_id(), 0);
  EntityID e1 = manager.CreateEntity();
  EXPECT_EQ(e1, 0);
}

TEST(EntityManagerTest, IsAlive) {
  EntityManager manager;
  EntityID e1 = manager.CreateEntity();
  EXPECT_TRUE(manager.IsAlive(e1));
  EXPECT_FALSE(manager.IsAlive(999));
  manager.DestroyEntity(e1);
  EXPECT_FALSE(manager.IsAlive(e1));
}

}  // namespace engine::ecs
