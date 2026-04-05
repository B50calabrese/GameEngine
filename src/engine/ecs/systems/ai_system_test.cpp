#include <gtest/gtest.h>
#include <engine/ecs/registry.h>
#include <engine/ecs/components/animation.h>
#include <engine/ecs/components/sprite.h>
#include <engine/ecs/components/lifetime.h>
#include <engine/ecs/components/waypoint_path.h>
#include <engine/ecs/components/transform.h>
#include <engine/ecs/systems/ai_system.h>

using namespace engine::ecs;
using namespace engine::ecs::components;
using namespace engine::ecs::systems;

TEST(AISystemTest, AnimationUpdate) {
    Registry registry;
    auto entity = registry.CreateEntity();

    Sprite sprite;
    sprite.sprite_index = 0;
    registry.AddComponent<Sprite>(entity, sprite);

    Animation anim;
    AnimationClip clip;
    clip.name = "idle";
    clip.frames = {{0, 0.1f}, {1, 0.1f}};
    clip.loop = true;
    anim.clips.push_back(clip);
    anim.current_clip = "idle";
    anim.is_playing = true;
    registry.AddComponent<Animation>(entity, anim);

    // Update with dt < duration
    AISystem::Update(&registry, 0.05f);
    EXPECT_EQ(registry.GetComponent<Sprite>(entity).sprite_index, 0);
    EXPECT_EQ(registry.GetComponent<Animation>(entity).current_frame, 0);

    // Update to trigger next frame
    AISystem::Update(&registry, 0.06f);
    EXPECT_EQ(registry.GetComponent<Sprite>(entity).sprite_index, 1);
    EXPECT_EQ(registry.GetComponent<Animation>(entity).current_frame, 1);

    // Update to trigger loop
    AISystem::Update(&registry, 0.11f);
    EXPECT_EQ(registry.GetComponent<Sprite>(entity).sprite_index, 0);
    EXPECT_EQ(registry.GetComponent<Animation>(entity).current_frame, 0);
}

TEST(AISystemTest, LifetimeUpdate) {
    Registry registry;
    auto entity = registry.CreateEntity();
    registry.AddComponent<Lifetime>(entity, {0.1f});

    EXPECT_TRUE(registry.IsAlive(entity));

    AISystem::Update(&registry, 0.05f);
    EXPECT_TRUE(registry.IsAlive(entity));

    AISystem::Update(&registry, 0.06f);
    EXPECT_FALSE(registry.IsAlive(entity));
}

TEST(AISystemTest, WaypointPathUpdate) {
    Registry registry;
    auto entity = registry.CreateEntity();
    registry.AddComponent<Transform>(entity, {{0.0f, 0.0f}});

    WaypointPath path;
    path.points = {{100.0f, 0.0f}, {100.0f, 100.0f}};
    path.speed = 100.0f;
    path.arrival_threshold = 1.0f;
    path.loop = false;
    registry.AddComponent<WaypointPath>(entity, path);

    // Move towards first waypoint
    AISystem::Update(&registry, 0.5f);
    auto& t = registry.GetComponent<Transform>(entity);
    EXPECT_NEAR(t.position.x, 50.0f, 0.01f);
    EXPECT_EQ(t.position.y, 0.0f);

    // Reach first waypoint
    AISystem::Update(&registry, 0.51f);
    EXPECT_NEAR(t.position.x, 100.0f, 1.1f); // Should be close to 100

    auto& p = registry.GetComponent<WaypointPath>(entity);
    EXPECT_EQ(p.current_index, 1);

    // Move towards second waypoint
    AISystem::Update(&registry, 0.5f);
    EXPECT_NEAR(t.position.y, 50.0f, 0.1f);
}
