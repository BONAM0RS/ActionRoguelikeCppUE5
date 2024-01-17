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
			Action->StopAction(Instigator);
			return true;
		}
	}

	return false;
}

