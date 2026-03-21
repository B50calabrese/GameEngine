#include <engine/ecs/ai_components.h>
#include <engine/ecs/ai_system.h>

namespace engine::ecs {

void AISystem::Update(Registry* registry, float dt) {
  if (!registry) {
    return;
  }
  // Update State Machines
  registry->ForEach<StateMachineComponent>(
      [dt](StateMachineComponent& comp) { comp.fsm.Update(dt); });

  // Update Behavior Trees
  registry->ForEach<BehaviorTreeComponent>(
      [dt](BehaviorTreeComponent& comp) { comp.tree.Tick(dt); });
}

}  // namespace engine::ecs
