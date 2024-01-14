// Fill out your copyright notice in the Description page of Project Settings.

#include "RLGameModeBase.h"

#include "EngineUtils.h"
#include "ActionRoguelike/ActorComponents/RLAttributeComponent.h"
#include "ActionRoguelike/AI/RLAICharacter.h"
#include "EnvironmentQuery/EnvQueryManager.h"


ARLGameModeBase::ARLGameModeBase()
	: DifficultyCurve(nullptr),
	  SpawnBotQuery(nullptr)
{
	SpawnTimerInterval = 2.0f;
}

void ARLGameModeBase::StartPlay()
{
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &ARLGameModeBase::SpawnBotTimerElapsed, SpawnTimerInterval, true);
}

void ARLGameModeBase::SpawnBotTimerElapsed()
{
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

	UE_LOG(LogTemp,Warning, TEXT("Found %i alive bots"), NumOfAliveBots);
	
	float MaxNumOfAliveBots = 10.f;
	if (DifficultyCurve != nullptr) {
		MaxNumOfAliveBots = DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);
	}
	
	if (NumOfAliveBots >= MaxNumOfAliveBots)
	{
		UE_LOG(LogTemp,Warning, TEXT("At max bot capacity. Skipping bot spawn"));
		return;
	}
	// end check
	
	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(
		this, SpawnBotQuery,this, EEnvQueryRunMode::RandomBest5Pct, nullptr);

	if (ensure(QueryInstance)) {
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ARLGameModeBase::OnQueryCompleted);
	}
	
}

void ARLGameModeBase::OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
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
