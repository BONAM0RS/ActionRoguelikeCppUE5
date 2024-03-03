/*
 * Third-person Action Roguelike game prototype made in Unreal Engine 5.3 as part of the Professional Game Development
 * in C++ and Unreal Engine course by Tom Looman.
 * Licensed for use only with UE based products.
 */

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "RLBTService_CheckAttackRange.generated.h"


UCLASS()
class ACTIONROGUELIKE_API URLBTService_CheckAttackRange : public UBTService
{
	GENERATED_BODY()

public:
	URLBTService_CheckAttackRange();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

protected:
	UPROPERTY(EditAnywhere, Category = "AI");
	FBlackboardKeySelector AttackRangeKey;

	UPROPERTY(EditAnywhere, Category = "AI");
	float WithingRange;
};
