// Fill out your copyright notice in the Description page of Project Settings.

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

	// if parameter is reference '&' then it will create output pin in blueprint node
	UFUNCTION(BlueprintCallable, Category = "GameplayLibrary")
	static bool ApplyDirectionalDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount,
		const FHitResult& HitResult);
};
