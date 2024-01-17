// Fill out your copyright notice in the Description page of Project Settings.

#include "RLAction.h"


void URLAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp,Warning,TEXT("Running: %s"), *GetNameSafe(this));
}

void URLAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp,Warning,TEXT("Stop: %s"), *GetNameSafe(this));
}

// Because it's UObject we should override this to have access to spawn actors, line traces etc functionality in blueprints
UWorld* URLAction::GetWorld() const
{
	// Outer is set when creating action via NewObject<T>
	UActorComponent* Comp = Cast<UActorComponent>(GetOuter());
	if (Comp != nullptr) {
		return Comp->GetWorld();
	}
	return nullptr;
}
