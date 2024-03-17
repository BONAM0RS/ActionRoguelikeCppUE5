/*
 * Third-person Action Roguelike game prototype made in Unreal Engine 5.3 as part of the Professional Game Development
 * in C++ and Unreal Engine course by Tom Looman.
 * Licensed for use only with UE based products.
 */

#include "RLHealPowerup.h"

#include "ActionRoguelike/Components/RLAttributeComponent.h"
#include "ActionRoguelike/Core/RLPlayerState.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(RLHealPowerup)

#define LOCTEXT_NAMESPACE "InteractableActors"


ARLHealPowerup::ARLHealPowerup()
{
	CreditsCost = 50;
}

void ARLHealPowerup::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!ensure(InstigatorPawn)) {
		return;
	}

	URLAttributeComponent* AttributeComp = URLAttributeComponent::GetAttributes(InstigatorPawn);
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

FText ARLHealPowerup::GetInteractText_Implementation(APawn* InstigatorPawn)
{
	URLAttributeComponent* AttributeComp = URLAttributeComponent::GetAttributes(InstigatorPawn);
	if (AttributeComp != nullptr && AttributeComp->IsFullHealth())
	{
		//return NSLOCTEXT("InteractableActors", "HealthPotion_FullHealthWarning", "Already at full health.");
		return LOCTEXT("HealthPotion_FullHealthWarning", "Already at full health.");
	}

	//return FText::Format(NSLOCTEXT("InteractableActors", "HealthPotion_InteractMessage", "Cost {0} Credits. Restores health to maximum."), CreditsCost);
	return FText::Format(LOCTEXT("HealthPotion_InteractMessage", "Cost {0} Credits. Restores health to maximum."), CreditsCost);
}

#undef LOCTEXT_NAMESPACE