/*
 * Third-person Action Roguelike game prototype made in Unreal Engine 5.3 as part of the Professional Game Development
 * in C++ and Unreal Engine course by Tom Looman.
 * Licensed for use only with UE based products.
 */

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "RLBTService_CheckHealth.generated.h"


UCLASS()
class ACTIONROGUELIKE_API URLBTService_CheckHealth : public UBTService
{
	GENERATED_BODY()

public:
	URLBTService_CheckHealth();
	
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

protected:
	UPROPERTY(EditAnywhere, Category = "AI");
	FBlackboardKeySelector LowHealthKey;

	UPROPERTY(EditAnywhere, Category = "AI");
	float LowHealthFraction;
};

