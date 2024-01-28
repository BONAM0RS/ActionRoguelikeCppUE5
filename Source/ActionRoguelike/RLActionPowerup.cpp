// Fill out your copyright notice in the Description page of Project Settings.


#include "RLActionPowerup.h"

#include "Actions/RLAction.h"
#include "ActorComponents/RLActionComponent.h"


ARLActionPowerup::ARLActionPowerup()
{
}

void ARLActionPowerup::Interact_Implementation(APawn* InstigatorPawn)
{
	// Make sure we have instigator & that action class was set up
	if (!ensure(InstigatorPawn && ActionToGrant))
	{
		return;
	}

	URLActionComponent* ActionComp = Cast<URLActionComponent>(InstigatorPawn->GetComponentByClass(URLActionComponent::StaticClass()));
	// Check if Player already has action class
	if (ActionComp)
	{
		if (ActionComp->GetAction(ActionToGrant) != nullptr)
		{
			//UE_LOG(LogTemp, Log, TEXT("Instigator already has action of class: %s"), *GetNameSafe(ActionToGrant));
			FString DebugMsg = FString::Printf(TEXT("Action '%s' already known."), *GetNameSafe(ActionToGrant));
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, DebugMsg);
			return;
		}

		// Give new Ability
		ActionComp->AddAction(ActionToGrant, InstigatorPawn);	
		HideAndCooldownPowerup();
	}
}
