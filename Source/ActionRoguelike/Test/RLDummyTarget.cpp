/*
 * Third-person Action Roguelike game prototype made in Unreal Engine 5.3 as part of the Professional Game Development
 * in C++ and Unreal Engine course by Tom Looman.
 * Licensed for use only with UE based products.
 */

#include "RLDummyTarget.h"

#include "ActionRoguelike/Components/RLAttributeComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(RLDummyTarget)


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
	// if (Delta < 0.0f)
	// {
		MeshComp->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);
	// }
}

