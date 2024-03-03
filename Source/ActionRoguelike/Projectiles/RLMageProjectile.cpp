// Fill out your copyright notice in the Description page of Project Settings.


#include "RLMageProjectile.h"

#include "ActionRoguelike/ActorComponents/RLActionComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "ActionRoguelike/Utility/RLGameplayFunctionLibrary.h"
#include "ActionRoguelike/Actions/RLAction_Effect.h"

ARLMageProjectile::ARLMageProjectile()
{
	ProjectileMovementComponent->InitialSpeed = 5000.0f;
	InitialLifeSpan = 10.f;

	DamageAmount = 20.0f;
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
		URLActionComponent* ActionComp = Cast<URLActionComponent>(OtherActor->GetComponentByClass(URLActionComponent::StaticClass()));
		if (ActionComp != nullptr && ActionComp->ActiveGameplayTags.HasTag(ParryTag))
		{
			ProjectileMovementComponent->Velocity = -ProjectileMovementComponent->Velocity;
			SetInstigator(Cast<APawn>(OtherActor));
			return;
		}

		if (URLGameplayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(), OtherActor, DamageAmount, SweepResult))
		{
			Explode();

			// Is Server?
			if (ActionComp && HasAuthority())
			{
				ActionComp->AddAction(BurningActionEffectClass, GetInstigator());
			}
		}
	}
}

void ARLMageProjectile::SetDamageAmount(float NewDamageAmount)
{
	DamageAmount = NewDamageAmount;
}

