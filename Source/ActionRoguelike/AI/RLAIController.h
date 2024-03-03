/*
 * Third-person Action Roguelike game prototype made in Unreal Engine 5.3 as part of the Professional Game Development
 * in C++ and Unreal Engine course by Tom Looman.
 * Licensed for use only with UE based products.
 */

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "RLAIController.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ARLAIController : public AAIController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UBehaviorTree* BehaviorTree;
};
