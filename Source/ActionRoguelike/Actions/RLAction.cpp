// Fill out your copyright notice in the Description page of Project Settings.

#include "RLAction.h"

//#include "ActionRoguelike/ActionRoguelike.h"
#include "ActionRoguelike/ActorComponents/RLActionComponent.h"
#include "Net/UnrealNetwork.h"


URLAction::URLAction()
	: ActionComp(nullptr),
	  Icon(nullptr),
	  TimeStarted(0.f)
{
	bAutoStart = false;
}

void URLAction::Initialize(URLActionComponent* NewActionComp)
{
	ActionComp = NewActionComp;
}

void URLAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp,Log,TEXT("Running: %s"), *GetNameSafe(this));
	//LogOnScreen(this, FString::Printf(TEXT("Started: %s"), *ActionName.ToString()), FColor::Green);
	
	ActionComp->ActiveGameplayTags.AppendTags(GrantsTags);

	RepData.bIsRunning = true;
	RepData.Instigator = Instigator;

	if (GetOwningComponent()->GetOwnerRole() == ROLE_Authority)
	{
		TimeStarted = GetWorld()->TimeSeconds;
	}

	GetOwningComponent()->OnActionStarted.Broadcast(GetOwningComponent(), this);
}

void URLAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp,Log,TEXT("Stop: %s"), *GetNameSafe(this));
	//LogOnScreen(this, FString::Printf(TEXT("Stopped: %s"), *ActionName.ToString()), FColor::White);

	// always check to be sure we don't call StopAction when bIsRunning = false, so it's already stopped;
	//ensureAlways(bIsRunning); // no need to check it on clients
	
	ActionComp->ActiveGameplayTags.RemoveTags(GrantsTags);

	RepData.bIsRunning = false;

	GetOwningComponent()->OnActionStopped.Broadcast(GetOwningComponent(), this);
}

bool URLAction::CanStart_Implementation(AActor* Instigator)
{
	if (RepData.bIsRunning) {
		return false;
	}
	
	if (ActionComp->ActiveGameplayTags.HasAny(BlockedTags)) {
		return false;
	}
	
	return true;
}

bool URLAction::IsRunning() const
{
	return RepData.bIsRunning;
}

// Because it's UObject we should override this to have access to spawn actors, line traces etc functionality in blueprints
UWorld* URLAction::GetWorld() const
{
	AActor* Actor = Cast<AActor>(GetOuter());
	if (Actor != nullptr) {
		return Actor->GetWorld();
	}
	return nullptr;
}

URLActionComponent* URLAction::GetOwningComponent() const
{
	return ActionComp;
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

void URLAction::OnRep_RepDataChanged()
{
	if (RepData.bIsRunning)
	{
		StartAction(RepData.Instigator);
	}
	else
	{
		StopAction(RepData.Instigator);
	}
}

void URLAction::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(URLAction, RepData);
	DOREPLIFETIME(URLAction, ActionComp);
	DOREPLIFETIME(URLAction, TimeStarted);
}
