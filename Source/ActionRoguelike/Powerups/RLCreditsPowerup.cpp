// Fill out your copyright notice in the Description page of Project Settings.

#include "RLCreditsPowerup.h"

#include "ActionRoguelike/Core/RLPlayerState.h"


ARLCreditsPowerup::ARLCreditsPowerup()
{
	CreditsAmount = 80;
}

void ARLCreditsPowerup::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!ensure(InstigatorPawn)) {
		return;
	}

	if (ARLPlayerState* PlayerState = InstigatorPawn->GetPlayerState<ARLPlayerState>())
	{
		PlayerState->AddCredits(CreditsAmount);
		HideAndCooldownPowerup();
	}
	
}
