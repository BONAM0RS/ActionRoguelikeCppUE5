// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "RLProjectileBase.h"
#include "RLMageProjectile.generated.h"


class URLAction_Effect;

UCLASS(Abstract)
class ACTIONROGUELIKE_API ARLMageProjectile : public ARLProjectileBase
{
	GENERATED_BODY()
	
public:
	ARLMageProjectile();

protected:
	virtual void PostInitializeComponents() override;
  
	UFUNCTION()
	void OnSphereComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

public:
	void SetDamageAmount(float NewDamageAmount);

protected:
	//ExposeOnSpawn allow to set property in spawn actor from class node
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ExposeOnSpawn="true"), Category = "Config")
	float DamageAmount;

	UPROPERTY(EditDefaultsOnly, Category = "Config")
	FGameplayTag ParryTag;

	UPROPERTY(EditDefaultsOnly, Category = "Config")
	TSubclassOf<URLAction_Effect> BurningActionEffectClass;
};
