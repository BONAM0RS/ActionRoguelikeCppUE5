// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "RLBTTaskNode_RangeAttack.generated.h"


UCLASS()
class ACTIONROGUELIKE_API URLBTTaskNode_RangeAttack : public UBTTaskNode
{
	GENERATED_BODY()

public:
	URLBTTaskNode_RangeAttack();
	
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere, Category = "AI")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "AI")
	float MaxBulletSpread;
};
