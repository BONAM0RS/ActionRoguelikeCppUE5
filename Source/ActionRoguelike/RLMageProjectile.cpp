// Fill out your copyright notice in the Description page of Project Settings.


#include "RLMageProjectile.h"

#include "ActorComponents/RLAttributeComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"


ARLMageProjectile::ARLMageProjectile()
{
	ProjectileMovementComponent->InitialSpeed = 5000.0f;
	InitialLifeSpan = 10.f;
}

void ARLMageProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ARLMageProjectile::OnSphereComponentBeginOverlap);
}

void ARLMageProjectile::OnSphereComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// don't forget to add check of instigator on hit event
	if (OtherActor != nullptr && OtherActor != GetInstigator())
	{
		UActorComponent* FoundActorComponent = OtherActor->GetComponentByClass(URLAttributeComponent::StaticClass());
		if (FoundActorComponent)
		{
			URLAttributeComponent* AttributeComponent = Cast<URLAttributeComponent>(FoundActorComponent);
			AttributeComponent->ApplyHealthChange(-20.f);

			//UE_LOG(LogTemp,Warning,TEXT("%S"), __FUNCTION__);
			Explode();
		}
	}
}