// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RLAction.generated.h"


UCLASS(Blueprintable)
class ACTIONROGUELIKE_API URLAction : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StartAction(AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StopAction(AActor* Instigator);

	virtual UWorld* GetWorld() const override;
	
public:
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	FName ActionName;

	
};
