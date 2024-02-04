// Fill out your copyright notice in the Description page of Project Settings.

#include "RLAnimInstance.h"

#include "GameplayTagContainer.h"
#include "ActorComponents/RLActionComponent.h"


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
