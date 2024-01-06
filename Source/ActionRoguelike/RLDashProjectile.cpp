// Fill out your copyright notice in the Description page of Project Settings.


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
	}
}
