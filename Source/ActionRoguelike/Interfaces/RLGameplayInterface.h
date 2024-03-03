/*
 * Third-person Action Roguelike game prototype made in Unreal Engine 5.3 as part of the Professional Game Development
 * in C++ and Unreal Engine course by Tom Looman.
 * Licensed for use only with UE based products.
 */

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RLGameplayInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class URLGameplayInterface : public UInterface
{
	GENERATED_BODY()
};


class ACTIONROGUELIKE_API IRLGameplayInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
	
public:
	// Called after the Actor state was restored from a SaveGame file
	UFUNCTION(BlueprintNativeEvent)
	void OnActorLoaded();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Interact(APawn* InstigatorPawn);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FText GetInteractText(APawn* InstigatorPawn);
};
