/*
 * Third-person Action Roguelike game prototype made in Unreal Engine 5.3 as part of the Professional Game Development
 * in C++ and Unreal Engine course by Tom Looman.
 * Licensed for use only with UE based products.
 */

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