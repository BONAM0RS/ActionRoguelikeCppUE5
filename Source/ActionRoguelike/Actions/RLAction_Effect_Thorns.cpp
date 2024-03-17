/*
 * Third-person Action Roguelike game prototype made in Unreal Engine 5.3 as part of the Professional Game Development
 * in C++ and Unreal Engine course by Tom Looman.
 * Licensed for use only with UE based products.
 */

#include "RLAction_Effect_Thorns.h"

#include "ActionRoguelike/Components/RLActionComponent.h"
#include "ActionRoguelike/Components/RLAttributeComponent.h"
#include "ActionRoguelike/Utility/RLGameplayFunctionLibrary.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(RLAction_Effect_Thorns)


URLAction_Effect_Thorns::URLAction_Effect_Thorns()
{
	Duration = 0.f;
	Period = 0.f;

	ReflectFraction = 0.2f;
}

void URLAction_Effect_Thorns::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);
	
	URLAttributeComponent* Attributes = URLAttributeComponent::GetAttributes(GetOwningComponent()->GetOwner());
	if (Attributes)
	{
		Attributes->OnHealthChanged.AddDynamic(this, &URLAction_Effect_Thorns::OnHealthChanged);
	}
}

void URLAction_Effect_Thorns::StopAction_Implementation(AActor* Instigator)
{
	Super::StopAction_Implementation(Instigator);

	URLAttributeComponent* Attributes = URLAttributeComponent::GetAttributes(GetOwningComponent()->GetOwner());
	if (Attributes)
	{
		Attributes->OnHealthChanged.RemoveDynamic(this, &URLAction_Effect_Thorns::OnHealthChanged);
	}
}

void URLAction_Effect_Thorns::OnHealthChanged(AActor* InstigatorActor, URLAttributeComponent* OwningComp,
	float NewHealth, float Delta)
{
	AActor* OwningActor = GetOwningComponent()->GetOwner();
	
	// Damage Only
	if (Delta < 0.0f && OwningActor != InstigatorActor)
	{
		// Round to nearest to avoid 'ugly' damage numbers and tiny reflections
		int32 ReflectedAmount = FMath::RoundToInt(Delta * ReflectFraction);
		if (ReflectedAmount == 0)
		{
			return;
		}

		// Flip to positive, so we don't end up healing ourselves when passed into damage
		ReflectedAmount = FMath::Abs(ReflectedAmount);

		// Return damage to sender
		URLGameplayFunctionLibrary::ApplyDamage(OwningActor, InstigatorActor, ReflectedAmount);
	}
}


