/*
 * Third-person Action Roguelike game prototype made in Unreal Engine 5.3 as part of the Professional Game Development
 * in C++ and Unreal Engine course by Tom Looman.
 * Licensed for use only with UE based products.
 */

#include "RLAnimInstance.h"

#include "GameplayTagContainer.h"
#include "ActionRoguelike/ActorComponents/RLActionComponent.h"


void URLAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	AActor* OwningActor = GetOwningActor();
	if (OwningActor != nullptr)
	{
		ActionComp = Cast<URLActionComponent>(OwningActor->GetComponentByClass(URLActionComponent::StaticClass()));
	}
}

void URLAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	static FGameplayTag StunnedTag = FGameplayTag::RequestGameplayTag("Status.Stunned");
	if (ActionComp != nullptr)
	{
		bIsStunned = ActionComp->ActiveGameplayTags.HasTag(StunnedTag);
	}
}
