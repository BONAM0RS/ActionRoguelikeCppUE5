/*
 * Third-person Action Roguelike game prototype made in Unreal Engine 5.3 as part of the Professional Game Development
 * in C++ and Unreal Engine course by Tom Looman.
 * Licensed for use only with UE based products.
 */

#include "RLAnimInstance.h"

#include "ActionRoguelike/Actions/RLAction_Effect.h"
#include "GameplayTagContainer.h"
#include "ActionRoguelike/Components/RLActionComponent.h"


void URLAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	AActor* OwningActor = GetOwningActor();
	if (OwningActor != nullptr)
	{
		ActionComp = Cast<URLActionComponent>(OwningActor->GetComponentByClass(URLActionComponent::StaticClass()));
	}

	bIsStunned = false;
	bIsTimeToEndStun = false;
	StunAnimDuration = 0.0f;

	if (StunAnimSequence != nullptr)
	{
		StunAnimDuration = StunAnimSequence->GetPlayLength();
	}
}

void URLAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	if (ActionComp != nullptr)
	{
		static FGameplayTag StunnedTag = FGameplayTag::RequestGameplayTag("Status.Stunned");
		bIsStunned = ActionComp->ActiveGameplayTags.HasTag(StunnedTag);
		if (bIsStunned)
		{
			URLAction_Effect* StunEffect = Cast<URLAction_Effect>(ActionComp->GetAction(StunningActionEffectClass));
			if (StunEffect->GetTimeRemaining() <= StunAnimDuration)
			{
				bIsTimeToEndStun = true;
			}
		}
		else
		{
			bIsTimeToEndStun = false;
		}
	}
}

