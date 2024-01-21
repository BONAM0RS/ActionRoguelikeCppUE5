// Fill out your copyright notice in the Description page of Project Settings.

#include "RLHealPowerup.h"

#include "ActorComponents/RLAttributeComponent.h"
#include "Core/RLPlayerState.h"


ARLHealPowerup::ARLHealPowerup()
{
	CreditsCost = 50;
}

void ARLHealPowerup::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!ensure(InstigatorPawn)) {
		return;
	}

	URLAttributeComponent* AttributeComp = Cast<URLAttributeComponent>(InstigatorPawn->GetComponentByClass(URLAttributeComponent::StaticClass()));
	// IsFullHealth is optional, because ApplyHealthChange return bool
	if (ensure(AttributeComp) && !AttributeComp->IsFullHealth())
	{
		if (ARLPlayerState* PlayerState = InstigatorPawn->GetPlayerState<ARLPlayerState>())
		{
			if (PlayerState->RemoveCredits(CreditsCost) && AttributeComp->ApplyHealthChange(this, AttributeComp->GetMaxHealth()))
			{
				HideAndCooldownPowerup();
			}
		}
		
	}
	
}
