/*
 * Third-person Action Roguelike game prototype made in Unreal Engine 5.3 as part of the Professional Game Development
 * in C++ and Unreal Engine course by Tom Looman.
 * Licensed for use only with UE based products.
 */


#include "RLActionPowerup.h"

#include "ActionRoguelike/Actions/RLAction.h"
#include "ActionRoguelike/Components/RLActionComponent.h"
#include "ActionRoguelike/Components/RLAttributeComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(RLActionPowerup)


#define LOCTEXT_NAMESPACE "InteractableActors"


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

	// Check if Player already has action class
	URLActionComponent* ActionComp = Cast<URLActionComponent>(InstigatorPawn->GetComponentByClass(URLActionComponent::StaticClass()));
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

FText ARLActionPowerup::GetInteractText_Implementation(APawn* InstigatorPawn)
{
	URLActionComponent* ActionComp = Cast<URLActionComponent>(InstigatorPawn->GetComponentByClass(URLActionComponent::StaticClass()));
	if (ActionComp != nullptr)
	{
		if (ActionComp->GetAction(ActionToGrant) != nullptr)
		{
			return LOCTEXT("ActionPowerup_Warning", "Black Hole action already known.");
		}
	}
	
	return LOCTEXT("ActionPowerup_InteractMessage", "Black Hole ability. It costs 5 rage. Use in direction with physic objects.");
}


#undef LOCTEXT_NAMESPACE