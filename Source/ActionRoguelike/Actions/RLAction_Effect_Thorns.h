/*
 * Third-person Action Roguelike game prototype made in Unreal Engine 5.3 as part of the Professional Game Development
 * in C++ and Unreal Engine course by Tom Looman.
 * Licensed for use only with UE based products.
 */

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


