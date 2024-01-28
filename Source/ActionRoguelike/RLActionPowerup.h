// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RLPowerupBase.h"
#include "RLActionPowerup.generated.h"

class URLAction;


UCLASS()
class ACTIONROGUELIKE_API ARLActionPowerup : public ARLPowerupBase
{
	GENERATED_BODY()

public:
	ARLActionPowerup();

	void Interact_Implementation(APawn* InstigatorPawn) override;
	
protected:
	UPROPERTY(EditAnywhere, Category = "Config")
	TSubclassOf<URLAction> ActionToGrant;
	
};
