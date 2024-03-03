/*
 * Third-person Action Roguelike game prototype made in Unreal Engine 5.3 as part of the Professional Game Development
 * in C++ and Unreal Engine course by Tom Looman.
 * Licensed for use only with UE based products.
 */

#include "RLDashProjectile.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"


ARLDashProjectile::ARLDashProjectile()
	: TeleportDelay(0.2f),
	  DetonateDelay(0.2f)
{
	ProjectileMovementComponent->InitialSpeed = 6000.f;
}

void ARLDashProjectile::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_DelayedDetonate, this, &ARLDashProjectile::Explode, DetonateDelay);
}

void ARLDashProjectile::Explode_Implementation()
{
	// Clear timer in case projectile hit something
	GetWorldTimerManager().ClearTimer(TimerHandle_DelayedDetonate);

	if (ensure(ImpactVFX)) {
		UGameplayStatics::SpawnEmitterAtLocation(this, ImpactVFX, GetActorLocation(), GetActorRotation());
	}

	ParticleSystemComponent->DeactivateSystem();
	
	ProjectileMovementComponent->StopMovementImmediately();
	SetActorEnableCollision(false);

	FTimerHandle TimerHandle_DelayedTeleport;
	GetWorldTimerManager().SetTimer(TimerHandle_DelayedTeleport, this, &ARLDashProjectile::TeleportInstigator, TeleportDelay);
}

void ARLDashProjectile::TeleportInstigator()
{
	AActor* ActorToTeleport = GetInstigator();
	if (ensure(ActorToTeleport))
	{
		ActorToTeleport->TeleportTo(GetActorLocation(), ActorToTeleport->GetActorRotation(), false, false);
		Destroy();
	}
}
