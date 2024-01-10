// Fill out your copyright notice in the Description page of Project Settings.

#include "RLHealPowerup.h"

#include "ActorComponents/RLAttributeComponent.h"


ARLHealPowerup::ARLHealPowerup()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp->SetupAttachment(RootComponent);
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
		if (AttributeComp->ApplyHealthChange(AttributeComp->GetMaxHealth()))
		{
			HideAndCooldownPowerup();
		}
	}
	
}
