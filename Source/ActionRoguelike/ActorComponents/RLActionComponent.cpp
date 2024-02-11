// Fill out your copyright notice in the Description page of Project Settings.

#include "RLActionComponent.h"

#include "ActionRoguelike/ActionRoguelike.h"
#include "ActionRoguelike/Actions/RLAction.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"


DECLARE_CYCLE_STAT(TEXT("StartActionByName"), STAT_StartActionByName, STATGROUP_ANDREW);


URLActionComponent::URLActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);
}

void URLActionComponent::BeginPlay()
{
	Super::BeginPlay();

	// Server only
	if (GetOwner()->HasAuthority())
	{
		for (TSubclassOf<URLAction> ActionClass : DefaultActions) {
			AddAction(ActionClass, GetOwner());
		}
	}
}

void URLActionComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	TArray<URLAction*> ActionsCopy = Actions;
	for (URLAction* Action : ActionsCopy)
	{
		if (Action && Action->IsRunning())
		{
			Action->StopAction(GetOwner());
		}
	}

	Super::EndPlay(EndPlayReason);
}

void URLActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//FString DebugMsg = GetNameSafe(GetOwner()) + " : " + ActiveGameplayTags.ToStringSimple();
	//GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::White, DebugMsg);

	// Draw All Actions
	// for (URLAction* Action : Actions)
	// {
	// 	FColor TextColor = Action->IsRunning() ? FColor::Blue : FColor::White;
	// 	FString ActionMsg = FString::Printf(TEXT("[%s] Action: %s"),*GetNameSafe(GetOwner()), *GetNameSafe(Action));
	// 	
	// 	LogOnScreen(this, ActionMsg, TextColor, 0.0f);
	// }
}

void URLActionComponent::AddAction(TSubclassOf<URLAction> ActionClass, AActor* Instigator)
{
	if (!ensure(ActionClass)) {
		return;
	}

	// Skip for clients
	if (!GetOwner()->HasAuthority())
	{
		UE_LOG(LogTemp,Warning,TEXT("Client attempting to AddAction. [Class: %s]"), *GetNameSafe(ActionClass));
		return;
	}

	URLAction* NewAction = NewObject<URLAction>(GetOwner(), ActionClass);
	if (ensure(NewAction))
	{
		NewAction->Initialize(this);
		
		Actions.Add(NewAction);

		if (NewAction->bAutoStart && ensure(NewAction->CanStart(Instigator)))
		{
			NewAction->StartAction(Instigator);
		}
	}
}

void URLActionComponent::RemoveAction(URLAction* ActionToRemove)
{
	if (!ensure(ActionToRemove != nullptr && !ActionToRemove->IsRunning()))
	{
		return;
	}
	
	Actions.Remove(ActionToRemove);
}

bool URLActionComponent::StartActionByName(AActor* Instigator, FName ActionName)
{
	SCOPE_CYCLE_COUNTER(STAT_StartActionByName);
	
	for (URLAction* Action : Actions)
	{
		if (Action != nullptr && Action->ActionName == ActionName)
		{
			if (!Action->CanStart(Instigator))
			{
				FString FailedMsg = FString::Printf(TEXT("Failed to run: %s"), *ActionName.ToString());
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FailedMsg);
				continue;
			}

			// Is Client?
			if (!GetOwner()->HasAuthority())
			{
				ServerStartAction(Instigator, ActionName);
			}

			// Bookmark for Unreal Insights
			TRACE_BOOKMARK(TEXT("StartAction::%s"), *GetNameSafe(Action));
			
			Action->StartAction(Instigator);
			return true;
		}
	}

	return false;
}

bool URLActionComponent::StopActionByName(AActor* Instigator, FName ActionName)
{
	for (URLAction* Action : Actions)
	{
		if (Action != nullptr && Action->ActionName == ActionName)
		{
			if (Action->IsRunning())
			{
				// Is Client?
				if (!GetOwner()->HasAuthority())
				{
					ServerStopAction(Instigator, ActionName);
				}
				
				Action->StopAction(Instigator);
				return true;
			}
		}
	}

	return false;
}

void URLActionComponent::ServerStartAction_Implementation(AActor* Instigator, FName ActionName)
{
	StartActionByName(Instigator, ActionName);
}

void URLActionComponent::ServerStopAction_Implementation(AActor* Instigator, FName ActionName)
{
	StopActionByName(Instigator, ActionName);
}

URLAction* URLActionComponent::GetAction(TSubclassOf<URLAction> ActionClass) const
{
	for (URLAction* Action : Actions)
	{
		if (Action && Action->IsA(ActionClass))
		{
			return Action;
		}
	}

	return nullptr;
}

bool URLActionComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	for (URLAction* Action : Actions)
	{
		if (Action)
		{
			WroteSomething |= Channel->ReplicateSubobject(Action, *Bunch, *RepFlags);
		}
	}

	return WroteSomething;
}

void URLActionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(URLActionComponent, Actions);
}

