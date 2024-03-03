/*
 * Third-person Action Roguelike game prototype made in Unreal Engine 5.3 as part of the Professional Game Development
 * in C++ and Unreal Engine course by Tom Looman.
 * Licensed for use only with UE based products.
 */

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

	UFUNCTION(BlueprintCallable, Category = "Action")
	float GetTimeRemaining() const;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config | Effect")
	float Duration;
	
	// Time between 'ticks' to apply effect
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config | Effect")
	float Period;

	FTimerHandle PeriodHandle;
	FTimerHandle DurationHandle;
};
