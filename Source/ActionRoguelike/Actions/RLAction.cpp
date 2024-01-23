// Fill out your copyright notice in the Description page of Project Settings.

#include "RLAction.h"

#include "ActionRoguelike/ActorComponents/RLActionComponent.h"


URLAction::URLAction()
{
	bAutoStart = false;
	bIsRunning = false;
}

void URLAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp,Warning,TEXT("Running: %s"), *GetNameSafe(this));

	URLActionComponent* ActionComp = GetOwningComponent();
	ActionComp->ActiveGameplayTags.AppendTags(GrantsTags);

	bIsRunning = true;
}

void URLAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp,Warning,TEXT("Stop: %s"), *GetNameSafe(this));

	// always check to be sure we don't call StopAction when bIsRunning = false, so it's already stopped;
	ensureAlways(bIsRunning);

	URLActionComponent* ActionComp = GetOwningComponent();
	ActionComp->ActiveGameplayTags.RemoveTags(GrantsTags);

	bIsRunning = false;
}

bool URLAction::CanStart_Implementation(AActor* Instigator)
{
	if (bIsRunning) {
		return false;
	}
		
	URLActionComponent* ActionComp = GetOwningComponent();
	if (ActionComp->ActiveGameplayTags.HasAny(BlockedTags)) {
		return false;
	}
	
	return true;
}

bool URLAction::IsRunning() const
{
	return bIsRunning;
}

// Because it's UObject we should override this to have access to spawn actors, line traces etc functionality in blueprints
UWorld* URLAction::GetWorld() const
{
	UActorComponent* Comp = GetOwningComponent();
	if (Comp != nullptr) {
		return Comp->GetWorld();
	}
	return nullptr;
}

void URLAction::SetWorldTimer(FTimerHandle& InOutHandle, FTimerDelegate const& InDelegate, float InRate, bool InbLoop,
	float InFirstDelay)
{
	GetWorld()->GetTimerManager().SetTimer(InOutHandle, InDelegate, InRate, InbLoop, InFirstDelay);
}

void URLAction::ClearWorldTimer(FTimerHandle& InHandle)
{
	GetWorld()->GetTimerManager().ClearTimer(InHandle);
}

URLActionComponent* URLAction::GetOwningComponent() const
{
	// Outer is set when creating action via NewObject<T>
	return Cast<URLActionComponent>(GetOuter());
}
