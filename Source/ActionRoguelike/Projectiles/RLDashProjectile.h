// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RLProjectileBase.h"
#include "RLDashProjectile.generated.h"


UCLASS(ABSTRACT)
class ACTIONROGUELIKE_API ARLDashProjectile : public ARLProjectileBase
{
	GENERATED_BODY()

public:
	ARLDashProjectile();

protected:
	virtual void BeginPlay() override;

	// Base class using BlueprintNativeEvent, so we must override the _Implementation, not just Explode()
	virtual void Explode_Implementation() override;

	void TeleportInstigator();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Config")
	float TeleportDelay;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Config")
	float DetonateDelay;

	FTimerHandle TimerHandle_DelayedDetonate;
};
