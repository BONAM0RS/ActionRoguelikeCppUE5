// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RLPowerupBase.h"
#include "RLHealPowerup.generated.h"


UCLASS(Abstract)
class ACTIONROGUELIKE_API ARLHealPowerup : public ARLPowerupBase
{
	GENERATED_BODY()

public:
	ARLHealPowerup();
	
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

	virtual FText GetInteractText_Implementation(APawn* InstigatorPawn) override;
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Config")
	int32 CreditsCost;
};
