/*
 * Third-person Action Roguelike game prototype made in Unreal Engine 5.3 as part of the Professional Game Development
 * in C++ and Unreal Engine course by Tom Looman.
 * Licensed for use only with UE based products.
 */

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
