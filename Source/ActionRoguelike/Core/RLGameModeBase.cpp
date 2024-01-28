// Fill out your copyright notice in the Description page of Project Settings.

#include "RLGameModeBase.h"

#include "EngineUtils.h"
#include "RLCharacter.h"
#include "RLPlayerState.h"
#include "ActionRoguelike/ActorComponents/RLAttributeComponent.h"
#include "ActionRoguelike/AI/RLAICharacter.h"
#include "EnvironmentQuery/EnvQueryManager.h"


static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("su.SpawnBots"), true,
	TEXT("Enable spawning of bots via timer"), ECVF_Cheat);


ARLGameModeBase::ARLGameModeBase()
	: PowerupSpawnQuery(nullptr),
	  DifficultyCurve(nullptr),
      SpawnBotQuery(nullptr)
{
	PlayerStateClass = ARLPlayerState::StaticClass();

	DesiredPowerupCount = 10;
	RequiredPowerupDistance = 2000;

	bSpawnBots = false;
	SpawnTimerInterval = 2.0f;

	CreditsPerKill = 20;
}

void ARLGameModeBase::StartPlay()
{
	Super::StartPlay();
	
	// Make sure we have assigned at least one power-up class
	if (ensure(PowerupClasses.Num() > 0))
	{
		// // Run EQS to find potential power-up spawn locations
		// UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this,
		// 	PowerupSpawnQuery, this, EEnvQueryRunMode::AllMatching, nullptr);
		// if (ensure(QueryInstance))
		// {
		// 	QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ARLGameModeBase::OnPowerupSpawnQueryCompleted);
		// }

		FEnvQueryRequest Request(PowerupSpawnQuery, this);
		Request.Execute(EEnvQueryRunMode::AllMatching, this, &ARLGameModeBase::OnPowerupSpawnQueryCompleted);
	}

	// start bot spawning
	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &ARLGameModeBase::SpawnBotTimerElapsed, SpawnTimerInterval, true);
}

void ARLGameModeBase::OnPowerupSpawnQueryCompleted(TSharedPtr<FEnvQueryResult> Result)
{
	FEnvQueryResult* QueryResult = Result.Get();
	if (!QueryResult->IsSuccessful())
	{
		UE_LOG(LogTemp, Warning, TEXT("Powerup EQS Query Failed!"));
		return;
	}

	// Retrieve all possible locations that passed the query
	TArray<FVector> Locations;
	QueryResult->GetAllAsLocations(Locations);

	// Keep used locations to easily check distance between points
	TArray<FVector> UsedLocations;

	int32 SpawnCounter = 0;
	// Break out if we reached the desired count or if we have no more potential positions remaining
	while (SpawnCounter < DesiredPowerupCount && Locations.Num() > 0)
	{
		// Pick a random location from remaining points.
		int32 RandomLocationIndex = FMath::RandRange(0, Locations.Num() - 1);

		FVector PickedLocation = Locations[RandomLocationIndex];
		// Remove to avoid picking again
		Locations.RemoveAt(RandomLocationIndex);

		// Check minimum distance requirement
		bool bValidLocation = true;
		for (FVector OtherLocation : UsedLocations)
		{
			float DistanceTo = (PickedLocation - OtherLocation).Size();

			if (DistanceTo < RequiredPowerupDistance)
			{
				// Show skipped locations due to distance
				//DrawDebugSphere(GetWorld(), PickedLocation, 50.0f, 20, FColor::Red, false, 10.0f);

				// too close, skip to next attempt
				bValidLocation = false;
				break;
			}
		}

		// Failed the distance test
		if (!bValidLocation)
		{
			continue;
		}

		// Pick a random powerup-class
		int32 RandomClassIndex = FMath::RandRange(0, PowerupClasses.Num() - 1);
		TSubclassOf<AActor> RandomPowerupClass = PowerupClasses[RandomClassIndex];

		GetWorld()->SpawnActor<AActor>(RandomPowerupClass, PickedLocation, FRotator::ZeroRotator);
		UE_LOG(LogTemp, Warning, TEXT("Spawn Powerup %i"), SpawnCounter);

		// Keep for distance checks
		UsedLocations.Add(PickedLocation);
		SpawnCounter++;
	}
}

// void ARLGameModeBase::OnPowerupSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
// {
// 	if (QueryStatus != EEnvQueryStatus::Success)
// 	{
// 		UE_LOG(LogTemp, Warning, TEXT("Powerup EQS Query Failed!"));
// 		return;
// 	}
//
// 	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();
//
// 	// Keep used locations to easily check distance between points
// 	TArray<FVector> UsedLocations;
//
// 	int32 SpawnCounter = 0;
// 	// Break out if we reached the desired count or if we have no more potential positions remaining
// 	while (SpawnCounter < DesiredPowerupCount && Locations.Num() > 0)
// 	{
// 		// Pick a random location from remaining points.
// 		int32 RandomLocationIndex = FMath::RandRange(0, Locations.Num() - 1);
//
// 		FVector PickedLocation = Locations[RandomLocationIndex];
// 		// Remove to avoid picking again
// 		Locations.RemoveAt(RandomLocationIndex);
//
// 		// Check minimum distance requirement
// 		bool bValidLocation = true;
// 		for (FVector OtherLocation : UsedLocations)
// 		{
// 			float DistanceTo = (PickedLocation - OtherLocation).Size();
//
// 			if (DistanceTo < RequiredPowerupDistance)
// 			{
// 				// Show skipped locations due to distance
// 				DrawDebugSphere(GetWorld(), PickedLocation, 50.0f, 20, FColor::Red, false, 10.0f);
//
// 				// too close, skip to next attempt
// 				bValidLocation = false;
// 				break;
// 			}
// 		}
//
// 		// Failed the distance test
// 		if (!bValidLocation)
// 		{
// 			continue;
// 		}
//
// 		// Pick a random powerup-class
// 		int32 RandomClassIndex = FMath::RandRange(0, PowerupClasses.Num() - 1);
// 		TSubclassOf<AActor> RandomPowerupClass = PowerupClasses[RandomClassIndex];
//
// 		GetWorld()->SpawnActor<AActor>(RandomPowerupClass, PickedLocation, FRotator::ZeroRotator);
// 		UE_LOG(LogTemp, Warning, TEXT("Spawn Powerup %i"), SpawnCounter);
//
// 		// Keep for distance checks
// 		UsedLocations.Add(PickedLocation);
// 		SpawnCounter++;
// 	}
// }

void ARLGameModeBase::SpawnBotTimerElapsed()
{
	if (!bSpawnBots)
	{
		UE_LOG(LogTemp,Warning,TEXT("Bot spawning disabled via GameMode settings"));
		return;
	}
	
	if (!CVarSpawnBots.GetValueOnGameThread())
	{
		UE_LOG(LogTemp,Warning,TEXT("Bot spawning disabled via 'CVarSpawnBots'"));
		return;
	}
	
	// I guess this entire check need own function
	
	int32 NumOfAliveBots = 0;
	//similar to GetAllActorsOfClass node in C++
	for (TActorIterator<ARLAICharacter> It(GetWorld()); It; ++It)
	{
		ARLAICharacter* Bot = *It;

		// if (Bot->IsAlive())
		URLAttributeComponent* AttributeComp = URLAttributeComponent::GetAttributes(Bot);
		if (ensure(AttributeComp) && AttributeComp->IsAlive()) {
			NumOfAliveBots++;
		}
	}

	//UE_LOG(LogTemp,Warning, TEXT("Found %i alive bots"), NumOfAliveBots);
	
	float MaxNumOfAliveBots = 10.f;
	if (DifficultyCurve != nullptr) {
		MaxNumOfAliveBots = DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);
	}
	
	if (NumOfAliveBots >= MaxNumOfAliveBots)
	{
		//UE_LOG(LogTemp,Warning, TEXT("At max bot capacity. Skipping bot spawn"));
		return;
	}
	// end check
	
	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(
		this, SpawnBotQuery,this, EEnvQueryRunMode::RandomBest5Pct, nullptr);

	if (ensure(QueryInstance)) {
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ARLGameModeBase::OnBotSpawnQueryCompleted);
	}
	
}

void ARLGameModeBase::OnBotSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if (!QueryStatus == EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp,Warning,TEXT("Spawn bot EQS Failed"));
		return;
	}
	
	TArray<FVector> QueryLocations = QueryInstance->GetResultsAsLocations();
	if (QueryLocations.Num() > 0)
	{
		GetWorld()->SpawnActor<AActor>(MinionClass, QueryLocations[0], FRotator::ZeroRotator);

		// track all the used spawn location
		DrawDebugSphere(GetWorld(), QueryLocations[0], 50.f, 20, FColor::Blue, false, 60.0f);
	}
}

void ARLGameModeBase::KillAll()
{
	for (TActorIterator<ARLAICharacter> It(GetWorld()); It; ++It)
	{
		ARLAICharacter* Bot = *It;

		// if (Bot->IsAlive())
		URLAttributeComponent* AttributeComp = URLAttributeComponent::GetAttributes(Bot);
		if (ensure(AttributeComp) && AttributeComp->IsAlive()) {
			AttributeComp->Kill(this); // maybe pass player for kill credit
		}
	}
}

void ARLGameModeBase::OnActorKilled(AActor* VictimActor, AActor* Killer)
{
	//UE_LOG(LogTemp,Warning, TEXT("OnActorKilled: Victim = %s, Killer = %s"), *GetNameSafe(VictimActor), *GetNameSafe(Killer));
	
	ARLCharacter* PlayerCharacter = Cast<ARLCharacter>(VictimActor);
	if (PlayerCharacter != nullptr)
	{
		FTimerHandle TimerHandle_RespawnDelay;

		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "RespawnPlayerElapsed", PlayerCharacter->GetController());

		float RespawnDelay = 2.0f;
		GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay, Delegate, RespawnDelay, false);
	}

	APawn* KillerPawn = Cast<APawn>(Killer);
	if (KillerPawn != nullptr)
	{
		ARLPlayerState* PlayerState = KillerPawn->GetPlayerState<ARLPlayerState>();
		if (PlayerState != nullptr)
		{
			PlayerState->AddCredits(CreditsPerKill);
		}
	}
}

void ARLGameModeBase::RespawnPlayerElapsed(AController* Controller)
{
	if (ensure(Controller))
	{
		Controller->UnPossess();
		RestartPlayer(Controller);
	}
}
