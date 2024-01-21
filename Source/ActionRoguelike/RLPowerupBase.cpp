// Fill out your copyright notice in the Description page of Project Settings.

#include "RLPowerupBase.h"

#include "Components/SphereComponent.h"


ARLPowerupBase::ARLPowerupBase()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetCollisionProfileName("Powerup");
	RootComponent = SphereComp;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp->SetupAttachment(RootComponent);

	RespawnTime = 10.f;
}

void ARLPowerupBase::Interact_Implementation(APawn* InstigatorPawn)
{
	// override in derived classes
}

void ARLPowerupBase::ShowPowerup()
{
	SetPowerupState(true);
}

void ARLPowerupBase::HideAndCooldownPowerup()
{
	SetPowerupState(false);

	GetWorldTimerManager().SetTimer(TimerHandle_RespawnTimer, this, &ARLPowerupBase::ShowPowerup, RespawnTime);
}

void ARLPowerupBase::SetPowerupState(bool bNewIsActive)
{
	SetActorEnableCollision(bNewIsActive);
	RootComponent->SetVisibility(bNewIsActive, true);
}





