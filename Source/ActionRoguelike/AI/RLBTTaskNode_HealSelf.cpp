// Fill out your copyright notice in the Description page of Project Settings.


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
