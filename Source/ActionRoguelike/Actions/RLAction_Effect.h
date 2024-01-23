// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RLAction.h"
#include "RLAction_Effect.generated.h"


UCLASS()
class ACTIONROGUELIKE_API URLAction_Effect : public URLAction
{
	GENERATED_BODY()

public:
	URLAction_Effect();

	void StartAction_Implementation(AActor* Instigator) override;
	
	void StopAction_Implementation(AActor* Instigator) override;
	
protected:
	UFUNCTION(BlueprintNativeEvent, Category = "Effect")
	void ExecutePeriodicEffect(AActor* Instigator);
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config | Effect")
	float Duration;

	// Time between 'ticks' to apply effect
	// TODO: add this description on hover property in details (same for other properties in all classes)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config | Effect")
	float Period;

	FTimerHandle PeriodHandle;
	FTimerHandle DurationHandle;
};
