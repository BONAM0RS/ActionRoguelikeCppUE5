// Fill out your copyright notice in the Description page of Project Settings.

#include "RLGameModeBase.h"

#include "EngineUtils.h"
#include "RLCharacter.h"
#include "RLPlayerState.h"
#include "ActionRoguelike/ActionRoguelike.h"
#include "ActionRoguelike/ActorComponents/RLActionComponent.h"
#include "ActionRoguelike/ActorComponents/RLAttributeComponent.h"
#include "ActionRoguelike/AI/RLAICharacter.h"
#include "ActionRoguelike/Data/RLMonsterDataAsset.h"
#include "ActionRoguelike/Interfaces/RLGameplayInterface.h"
#include "ActionRoguelike/SaveGame/RLSaveGame.h"
#include "ActionRoguelike/SaveGame/RLSaveGameSubsystem.h"
#include "Engine/AssetManager.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(RLGameModeBase)


static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("su.SpawnBots"), true,
                                                TEXT("Enable spawning of bots via timer"), ECVF_Cheat);


ARLGameModeBase::ARLGameModeBase()
	: //CurrentSaveGame(nullptr),
	  PowerupSpawnQuery(nullptr),
	  DifficultyCurve(nullptr),
	  SpawnBotQuery(nullptr),
	  MonsterTable(nullptr)
{
	PlayerStateClass = ARLPlayerState::StaticClass();

	DesiredPowerupCount = 10;
	RequiredPowerupDistance = 2000;

	bSpawnBots = false;
	SpawnTimerInterval = 2.0f;

	CreditsPerKill = 20;

	//SlotName = "SaveGame_01";
}

void ARLGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	// (Save/Load logic moved into new SaveGameSubsystem)
	URLSaveGameSubsystem* SaveGameSubsystem = GetGameInstance()->GetSubsystem<URLSaveGameSubsystem>();

	// Optional slot name (Falls back to slot specified in SaveGameSettings class/INI otherwise)
	FString SelectedSaveSlot = UGameplayStatics::ParseOption(Options, "SaveGame");
	SaveGameSubsystem->LoadSaveGame(SelectedSaveSlot);
}

// void ARLGameModeBase::WriteSaveGame()
// {
// 	// Iterate all player states, we don't have proper ID to match yet (requires Steam or EOS)
// 	for (int32 i = 0; i < GameState->PlayerArray.Num(); i ++)
// 	{
// 		ARLPlayerState* PlayerState = Cast<ARLPlayerState>(GameState->PlayerArray[i]);
// 		if (PlayerState != nullptr)
// 		{
// 			PlayerState->SavePlayerState(CurrentSaveGame);
// 			break; // single player only at this point
// 		}
// 	}
//
// 	CurrentSaveGame->SavedActors.Empty();
//
// 	// Iterate the entire world of actors
// 	for (FActorIterator It(GetWorld()); It ; ++It)
// 	{
// 		AActor* Actor = *It;
// 		//Only interested in our 'gameplay actors'
// 		if (!Actor->Implements<URLGameplayInterface>())
// 		{
// 			continue;
// 		}
//
// 		FActorSaveData ActorData;
// 		ActorData.ActorName = Actor->GetName();
// 		ActorData.Transform = Actor->GetActorTransform();
//
// 		// Pass the array to fill with data from Actor
// 		FMemoryWriter MemWriter(ActorData.ByteData);
// 		
// 		FObjectAndNameAsStringProxyArchive Archive(MemWriter, true);
//
// 		//Find only variables with UPROPERTY(SaveGame)
// 		Archive.ArIsSaveGame = true;
//
// 		// Converts Actor's SaveGame UPROPERTY variables into binary array
// 		Actor->Serialize(Archive);
//
// 		CurrentSaveGame->SavedActors.Add(ActorData);
// 	}
// 	
// 	UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SlotName, 0);
// }

// void ARLGameModeBase::LoadSaveGame()
// {
// 	if (UGameplayStatics::DoesSaveGameExist(SlotName, 0))
// 	{
// 		CurrentSaveGame = Cast<URLSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
// 		if (CurrentSaveGame == nullptr)
// 		{
// 			UE_LOG(LogTemp,Warning,TEXT("Failed to load SaveGame Data"));
// 			return;
// 		}
//
// 		UE_LOG(LogTemp,Log,TEXT("Loaded SaveGame Data"));
//
// 		// Iterate the entire world of actors
// 		for (FActorIterator It(GetWorld()); It ; ++It)
// 		{
// 			AActor* Actor = *It;
// 			//Only interested in our 'gameplay actors'
// 			if (!Actor->Implements<URLGameplayInterface>())
// 			{
// 				continue;
// 			}
//
// 			for (FActorSaveData ActorData : CurrentSaveGame->SavedActors)
// 			{
// 				if (ActorData.ActorName == Actor->GetName())
// 				{
// 					Actor->SetActorTransform(ActorData.Transform);
//
// 					//
// 					FMemoryReader MemReader(ActorData.ByteData);
// 					FObjectAndNameAsStringProxyArchive Archive(MemReader, true);
//
// 					//Find only variables with UPROPERTY(SaveGame)
// 					Archive.ArIsSaveGame = true;
//
// 					// Converts binary array back into Actor's SaveGame UPROPERTY variables
// 					Actor->Serialize(Archive);
//
// 					IRLGameplayInterface::Execute_OnActorLoaded(Actor);
// 					
// 					break;
// 				}
// 			}
// 		}
// 	}
// 	else
// 	{
// 		CurrentSaveGame = Cast<URLSaveGame>(UGameplayStatics::CreateSaveGameObject(URLSaveGame::StaticClass()));
// 		UE_LOG(LogTemp,Log,TEXT("Created New SaveGame Data"));
// 	}
// }

void ARLGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	// // Calling Before Super::, so we set variables before 'BeginPlayingState' is called in PlayerController (which is where we init UI in player controller)
	// ARLPlayerState* PlayerState = NewPlayer->GetPlayerState<ARLPlayerState>();
	// if (PlayerState != nullptr)
	// {
	// 	PlayerState->LoadPlayerState(CurrentSaveGame);
	// }
	//
	// Super::HandleStartingNewPlayer_Implementation(NewPlayer);

	// Calling Before Super:: so we set variables before 'beginplayingstate' is called in PlayerController (which is where we instantiate UI)
	URLSaveGameSubsystem* SaveGameSubsystem = GetGameInstance()->GetSubsystem<URLSaveGameSubsystem>();
	SaveGameSubsystem->HandleStartingNewPlayer(NewPlayer);

	Super::HandleStartingNewPlayer_Implementation(NewPlayer);

	// Now we're ready to override spawn location
	// Alternatively we could override core spawn location to use store locations immediately (skipping the whole 'find player start' logic)
	SaveGameSubsystem->OverrideSpawnTransform(NewPlayer);
}

void ARLGameModeBase::StartPlay()
{
	Super::StartPlay();
	
	// Make sure we have assigned at least one power-up class
	if (ensure(PowerupClasses.Num() > 0))
	{
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
		//UE_LOG(LogTemp, Warning, TEXT("Spawn Powerup %i"), SpawnCounter);

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
	//LogOnScreen(this, "SpawnBotTimerElapsed");
	
	if (!bSpawnBots)
	{
		//LogOnScreen(this, "Bot spawning disabled via GameMode settings", FColor::Orange);
		UE_LOG(LogTemp,Warning,TEXT("Bot spawning disabled via GameMode settings"));
		return;
	}
	
	if (!CVarSpawnBots.GetValueOnGameThread())
	{
		//LogOnScreen(this, "Bot spawning disabled via 'CVarSpawnBots'", FColor::Orange);
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
		//LogOnScreen(this, "Max Bot capacity. Skipping bot spawn", FColor::Orange);
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
		LogOnScreen(this, "Spawn bot EQS Failed", FColor::Red);
		UE_LOG(LogTemp,Warning,TEXT("Spawn bot EQS Failed"));
		return;
	}
	
	TArray<FVector> QueryLocations = QueryInstance->GetResultsAsLocations();
	if (QueryLocations.Num() > 0)
	{
		if (MonsterTable != nullptr)
		{
			TArray<FMonsterInfoRow*> Rows;
			MonsterTable->GetAllRows("", Rows);

			// Get Random Enemy
			int32 RandomIndex = FMath::RandRange(0, Rows.Num() - 1);
			FMonsterInfoRow* SelectedRow = Rows[RandomIndex];

			UAssetManager* AssetManager = UAssetManager::GetIfInitialized();
			if (AssetManager)
			{
				LogOnScreen(this, "Loading monster ...", FColor::Green);
				
				TArray<FName> Bundles;

				FStreamableDelegate Delegate = FStreamableDelegate::CreateUObject(this, &ARLGameModeBase::OnMonsterLoaded,
					SelectedRow->MonsterId, QueryLocations[0]);
				
				AssetManager->LoadPrimaryAsset(SelectedRow->MonsterId, Bundles, Delegate);
			}
		}
	}
	else
	{
		LogOnScreen(this, "QueryLocations < 0", FColor::Red);
	}
}

void ARLGameModeBase::OnMonsterLoaded(FPrimaryAssetId LoadedId, FVector SpawnLocation)
{
	LogOnScreen(this, "Finished loading.", FColor::Green);
	
	UAssetManager* AssetManager = UAssetManager::GetIfInitialized();
	if (AssetManager)
	{
		URLMonsterDataAsset* MonsterDataAsset = Cast<URLMonsterDataAsset>(AssetManager->GetPrimaryAssetObject(LoadedId));
		if (MonsterDataAsset)
		{
			AActor* NewBot = GetWorld()->SpawnActor<AActor>(MonsterDataAsset->MonsterClass, SpawnLocation, FRotator::ZeroRotator);
			if (NewBot != nullptr)
			{
				LogOnScreen(this, FString::Printf(TEXT("Spawned enemy: %s (%s)"), *GetNameSafe(NewBot), *GetNameSafe(MonsterDataAsset)));
	
				// Grant special actions, buffs, etc.
				URLActionComponent* ActionComp = Cast<URLActionComponent>(NewBot->GetComponentByClass(URLActionComponent::StaticClass()));
				if (ActionComp != nullptr)
				{
					for (TSubclassOf<URLAction> ActionClass : MonsterDataAsset->Actions)
					{
						ActionComp->AddAction(ActionClass, NewBot);
					}
				}
			}

			// track all the used spawn location
			//DrawDebugSphere(GetWorld(), QueryLocations[0], 50.f, 20, FColor::Blue, false, 60.0f);
		}
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

		// Store time if it was better than previous record
		ARLPlayerState* PlayerState = PlayerCharacter->GetPlayerState<ARLPlayerState>();
		if (PlayerState)
		{
			PlayerState->UpdatePersonalRecord(GetWorld()->TimeSeconds);
		}

		URLSaveGameSubsystem* SaveGameSubsystem= GetGameInstance()->GetSubsystem<URLSaveGameSubsystem>();
		// Immediately auto save on death
		SaveGameSubsystem->WriteSaveGame();
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


