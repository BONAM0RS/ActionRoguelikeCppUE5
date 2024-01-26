// Fill out your copyright notice in the Description page of Project Settings.

#include "RLActionComponent.h"

#include "ActionRoguelike/Actions/RLAction.h"


URLActionComponent::URLActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);
}

void URLActionComponent::BeginPlay()
{
	Super::BeginPlay();

	for (TSubclassOf<URLAction> ActionClass : DefaultActions) {
		AddAction(ActionClass, GetOwner());
	}
}

void URLActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FString DebugMsg = GetNameSafe(GetOwner()) + " : " + ActiveGameplayTags.ToStringSimple();
	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::White, DebugMsg);
}

void URLActionComponent::AddAction(TSubclassOf<URLAction> ActionClass, AActor* Instigator)
{
	if (!ensure(ActionClass)) {
		return;
	}

	URLAction* NewAction = NewObject<URLAction>(this, ActionClass);
	if (ensure(NewAction))
	{
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
	for (URLAction* Action : Actions)
	{
		if (Action != nullptr && Action->ActionName == ActionName)
		{
			//TODO: maybe get rif of continue
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

