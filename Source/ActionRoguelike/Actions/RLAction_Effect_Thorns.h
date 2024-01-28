// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RLAction_Effect.h"
#include "RLAction_Effect_Thorns.generated.h"


class URLAttributeComponent;

UCLASS()
class ACTIONROGUELIKE_API URLAction_Effect_Thorns : public URLAction_Effect
{
	GENERATED_BODY()

public:
	URLAction_Effect_Thorns();
	
	void StartAction_Implementation(AActor* Instigator) override;
	
	void StopAction_Implementation(AActor* Instigator) override;

protected:
	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, URLAttributeComponent* OwningComp, float NewHealth, float Delta);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Thorns")
	float ReflectFraction;
	
};


