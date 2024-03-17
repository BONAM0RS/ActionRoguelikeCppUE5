/*
 * Third-person Action Roguelike game prototype made in Unreal Engine 5.3 as part of the Professional Game Development
 * in C++ and Unreal Engine course by Tom Looman.
 * Licensed for use only with UE based products.
 */

#include "RLCreditsPowerup.h"

#include "ActionRoguelike/Core/RLPlayerState.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(RLCreditsPowerup)


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
