// Fill out your copyright notice in the Description page of Project Settings.

#include "RLDummyTarget.h"

#include "ActionRoguelike/ActorComponents/RLAttributeComponent.h"


ARLDummyTarget::ARLDummyTarget()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	RootComponent = MeshComp;

	AttributeComp = CreateDefaultSubobject<URLAttributeComponent>("AttributeComp");
}

void ARLDummyTarget::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	AttributeComp->OnHealthChanged.AddDynamic(this, &ARLDummyTarget::OnHealthChanged);
}

void ARLDummyTarget::OnHealthChanged(AActor* InstigatorActor, URLAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	//if (Delta < 0.0f)
	//{
		MeshComp->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);
	//}
}

