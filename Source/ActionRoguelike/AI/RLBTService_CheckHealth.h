// Fill out your copyright notice in the Description page of Project Settings.

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

