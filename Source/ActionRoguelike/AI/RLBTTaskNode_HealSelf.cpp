/*
 * Third-person Action Roguelike game prototype made in Unreal Engine 5.3 as part of the Professional Game Development
 * in C++ and Unreal Engine course by Tom Looman.
 * Licensed for use only with UE based products.
 */


#include "RLBTTaskNode_HealSelf.h"

#include "AIController.h"
#include "ActionRoguelike/ActorComponents/RLAttributeComponent.h"

EBTNodeResult::Type URLBTTaskNode_HealSelf::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* AIPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (AIPawn == nullptr) {
		return EBTNodeResult::Failed;
	}

	URLAttributeComponent* AttributeComp = URLAttributeComponent::GetAttributes(AIPawn);
	if (ensure(AttributeComp)) {
		AttributeComp->ApplyHealthChange(AIPawn, AttributeComp->GetMaxHealth());
	}

	return EBTNodeResult::Succeeded;
}
