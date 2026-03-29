#include <engine/ecs/components/ai_components.h>
#include <engine/ecs/systems/ai_system.h>

namespace engine::ecs::systems {

void AISystem::Update(Registry* registry, float dt) {
  if (!registry) {
    return;
  }
  // Update State Machines
  registry->ForEach<engine::ecs::components::StateMachine>(
      [dt](engine::ecs::components::StateMachine& comp) {
        comp.fsm.Update(dt);
      });

  // Update Behavior Trees
  registry->ForEach<engine::ecs::components::BehaviorTree>(
      [dt](engine::ecs::components::BehaviorTree& comp) {
        comp.tree.Tick(dt);
      });
}

}  // namespace engine::ecs::systems
