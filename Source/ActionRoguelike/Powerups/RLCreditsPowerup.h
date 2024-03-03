// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RLPowerupBase.h"
#include "RLCreditsPowerup.generated.h"


UCLASS()
class ACTIONROGUELIKE_API ARLCreditsPowerup : public ARLPowerupBase
{
	GENERATED_BODY()

public:
	ARLCreditsPowerup();
	
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Config")
	int32 CreditsAmount;
	
};
