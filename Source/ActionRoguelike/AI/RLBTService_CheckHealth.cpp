/*
 * Third-person Action Roguelike game prototype made in Unreal Engine 5.3 as part of the Professional Game Development
 * in C++ and Unreal Engine course by Tom Looman.
 * Licensed for use only with UE based products.
 */

#include "RLBTService_CheckHealth.h"

#include "AIController.h"
#include "ActionRoguelike/ActorComponents/RLAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"


URLBTService_CheckHealth::URLBTService_CheckHealth()
{
	LowHealthFraction = 0.3f;
}

void URLBTService_CheckHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* AIPawn = OwnerComp.GetAIOwner()->GetPawn();
	if(ensure(AIPawn))
	{
		URLAttributeComponent* AttributeComp = URLAttributeComponent::GetAttributes(AIPawn);
		if (ensure(AttributeComp))
		{
			float HealthPercent = AttributeComp->GetCurrentHealth() / AttributeComp->GetMaxHealth();
			bool bLowHealth = HealthPercent < LowHealthFraction;

			UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
			BlackboardComp->SetValueAsBool(LowHealthKey.SelectedKeyName, bLowHealth);
		}
	}
}
