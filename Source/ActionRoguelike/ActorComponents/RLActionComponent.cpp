// Fill out your copyright notice in the Description page of Project Settings.

#include "RLActionComponent.h"

#include "ActionRoguelike/Actions/RLAction.h"


URLActionComponent::URLActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void URLActionComponent::BeginPlay()
{
	Super::BeginPlay();

	for (TSubclassOf<URLAction> ActionClass : DefaultActions) {
		AddAction(ActionClass);
	}
}

void URLActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FString DebugMsg = GetNameSafe(GetOwner()) + " : " + ActiveGameplayTags.ToStringSimple();
	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::White, DebugMsg);
}

void URLActionComponent::AddAction(TSubclassOf<URLAction> ActionClass)
{
	if (!ensure(ActionClass)) {
		return;
	}

	URLAction* NewAction = NewObject<URLAction>(this, ActionClass);
	if (ensure(NewAction))
	{
		Actions.Add(NewAction);
	}
}

bool URLActionComponent::StartActionByName(AActor* Instigator, FName ActionName)
{
	for (URLAction* Action : Actions)
	{
		if (Action != nullptr && Action->ActionName == ActionName)
		{
			// maybe get rif of continue
			if (!Action->CanStart(Instigator))
			{
				FString FailedMsg = FString::Printf(TEXT("Failed to run: %s"), *ActionName.ToString());
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FailedMsg);
				continue;
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

