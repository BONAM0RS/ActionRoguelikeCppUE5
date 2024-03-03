/*
 * Third-person Action Roguelike game prototype made in Unreal Engine 5.3 as part of the Professional Game Development
 * in C++ and Unreal Engine course by Tom Looman.
 * Licensed for use only with UE based products.
 */

#include "RLBTService_CheckAttackRange.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"


URLBTService_CheckAttackRange::URLBTService_CheckAttackRange()
{
	WithingRange = 2000.f;
}

void URLBTService_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// Check distance between AI pawn and Target Actor
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (ensure(BlackboardComp))
	{
		AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject("TargetActor"));
		if (TargetActor != nullptr)
		{
			AAIController* AIController = OwnerComp.GetAIOwner();
			if (ensure(AIController))
			{
				APawn* AIPawn = AIController->GetPawn();
				if (ensure(AIPawn))
				{
					float DistanceToTarget = FVector::Distance(TargetActor->GetActorLocation(), AIPawn->GetActorLocation());
					bool bWithingRange = DistanceToTarget < WithingRange;

					bool bHasLOS = false; 
					if (bWithingRange) {
						bHasLOS = AIController->LineOfSightTo(TargetActor);
					}
					
					BlackboardComp->SetValueAsBool(AttackRangeKey.SelectedKeyName, (bWithingRange && bHasLOS));
				}
			}
		}
		
	}
}
