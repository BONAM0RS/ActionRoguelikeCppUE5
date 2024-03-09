/*
 * Third-person Action Roguelike game prototype made in Unreal Engine 5.3 as part of the Professional Game Development
 * in C++ and Unreal Engine course by Tom Looman.
 * Licensed for use only with UE based products.
 */

#include "RLMageProjectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "ActionRoguelike/Utility/RLGameplayFunctionLibrary.h"
#include "ActionRoguelike/Actions/RLAction_Effect.h"
#include "ActionRoguelike/Components/RLActionComponent.h"
#include "ActionRoguelike/Core/RLCharacter.h"


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
	// Don't forget to add check of instigator on hit event
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
				static FGameplayTag BurningTag = FGameplayTag::RequestGameplayTag("Status.Burning");
				if (!ActionComp->ActiveGameplayTags.HasTag(BurningTag))
				{
					ActionComp->AddAction(BurningActionEffectClass, GetInstigator());
				}

				
				int randomNum = FMath::RandRange(0,4);
				if (randomNum == 4)
				{
					static FGameplayTag StunnedTag = FGameplayTag::RequestGameplayTag("Status.Stunned");
					if (!ActionComp->ActiveGameplayTags.HasTag(StunnedTag))
					{
						// If it's player
						if (Cast<ARLCharacter>(OtherActor))
						{
							ActionComp->AddAction(StunningActionEffectClass, GetInstigator());
						}
					}
				}
			}
		}
	}
}

void ARLMageProjectile::OnSphereComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherComp != nullptr && OtherComp->IsSimulatingPhysics()) {
		URLGameplayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(), OtherActor, DamageAmount, Hit, 100000.f);
	}

	Super::OnSphereComponentHit(HitComponent, OtherActor, OtherComp, NormalImpulse, Hit);
}

void ARLMageProjectile::SetDamageAmount(float NewDamageAmount)
{
	DamageAmount = NewDamageAmount;
}

