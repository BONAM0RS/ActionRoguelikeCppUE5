// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "GameFramework/GameModeBase.h"
#include "RLGameModeBase.generated.h"

class UEnvQueryInstanceBlueprintWrapper;
class UEnvQuery;


UCLASS()
class ACTIONROGUELIKE_API ARLGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:
	ARLGameModeBase();
	
	virtual void StartPlay() override;

	UFUNCTION()
	void SpawnBotTimerElapsed();

	UFUNCTION()
	void OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

public:
	//Console command
	UFUNCTION(Exec)
	void KillAll();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UCurveFloat* DifficultyCurve;
	
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UEnvQuery* SpawnBotQuery;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TSubclassOf<AActor> MinionClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float SpawnTimerInterval;
	
	FTimerHandle TimerHandle_SpawnBots;
	
};
