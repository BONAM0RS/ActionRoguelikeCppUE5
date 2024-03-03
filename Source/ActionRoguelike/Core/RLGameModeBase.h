// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "GameFramework/GameModeBase.h"
#include "RLGameModeBase.generated.h"

class URLMonsterDataAsset;
class URLSaveGame;
class UEnvQueryInstanceBlueprintWrapper;
class UEnvQuery;


// DataTable Row for spawning monsters in game mode
USTRUCT(BlueprintType)
struct FMonsterInfoRow : public FTableRowBase 
{
	GENERATED_BODY()

public:
	FMonsterInfoRow()
	{
		Weight = 1.0f;
		SpawnCost = 5.0f;
		KillReward = 20.0f;
	}

	// UPROPERTY(EditAnywhere, BlueprintReadOnly)
	// TSubclassOf<AActor> MonsterClass;

	// UPROPERTY(EditAnywhere, BlueprintReadOnly)
	// URLMonsterDataAsset* MonsterData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FPrimaryAssetId MonsterId;

	// Relative chance to pick this monster
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Weight;

	// Points required by game mode to spawn this unit
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SpawnCost;

	// Amount of credits awarded to killer of this unit
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float KillReward;
};


UCLASS()
class ACTIONROGUELIKE_API ARLGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:
	ARLGameModeBase();

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

public:
	// UFUNCTION(BlueprintCallable, Category = "SaveGame")
	// void WriteSaveGame();
	//
	// void LoadSaveGame();

	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

protected:
	virtual void StartPlay() override;

	// UFUNCTION()
	// void OnPowerupSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	void OnPowerupSpawnQueryCompleted(TSharedPtr<FEnvQueryResult> Result);

	UFUNCTION()
	void SpawnBotTimerElapsed();

	UFUNCTION()
	void OnBotSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	void OnMonsterLoaded(FPrimaryAssetId LoadedId, FVector SpawnLocation);

public:
	//Console command
	UFUNCTION(Exec)
	void KillAll();

	virtual void OnActorKilled(AActor* VictimActor, AActor* Killer);

protected:
	UFUNCTION()
	void RespawnPlayerElapsed(AController* Controller);

protected:
	//FString SlotName;
	
	// UPROPERTY()
	// URLSaveGame* CurrentSaveGame;
	
	UPROPERTY(EditDefaultsOnly, Category = "Config | Powerups")
	UEnvQuery* PowerupSpawnQuery;

	/* All power-up classes used to spawn with EQS at match start */
	UPROPERTY(EditDefaultsOnly, Category = "Config | Powerups")
	TArray<TSubclassOf<AActor>> PowerupClasses;

	/* Distance required between power-up spawn locations */
	UPROPERTY(EditDefaultsOnly, Category = "Config | Powerups")
	float RequiredPowerupDistance;

	/* Amount of powerups to spawn during match start */
	UPROPERTY(EditDefaultsOnly, Category = "Config | Powerups")
	int32 DesiredPowerupCount;

	UPROPERTY(EditDefaultsOnly, Category = "Config | AI")
	bool bSpawnBots;
	
	UPROPERTY(EditDefaultsOnly, Category = "Config | AI")
	UCurveFloat* DifficultyCurve;
	
	UPROPERTY(EditDefaultsOnly, Category = "Config | AI")
	UEnvQuery* SpawnBotQuery;

	UPROPERTY(EditDefaultsOnly, Category = "Config | AI")
	UDataTable* MonsterTable;
		
	// UPROPERTY(EditDefaultsOnly, Category = "Config | AI")
	// TSubclassOf<AActor> MinionClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Config | AI")
	float SpawnTimerInterval;
	
	FTimerHandle TimerHandle_SpawnBots;
	
	UPROPERTY(EditDefaultsOnly, Category = "Config | Credits")
	int32 CreditsPerKill;


};
