/*
 * Third-person Action Roguelike game prototype made in Unreal Engine 5.3 as part of the Professional Game Development
 * in C++ and Unreal Engine course by Tom Looman.
 * Licensed for use only with UE based products.
 */

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RLGameplayFunctionLibrary.generated.h"


UCLASS()
class ACTIONROGUELIKE_API URLGameplayFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "GameplayLibrary")
	static bool ApplyDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount);

	// If parameter is reference '&' then it will create output pin in blueprint node
	UFUNCTION(BlueprintCallable, Category = "GameplayLibrary")
	static bool ApplyDirectionalDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount,
		const FHitResult& HitResult);
};
