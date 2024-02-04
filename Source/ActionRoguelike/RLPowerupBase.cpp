// Fill out your copyright notice in the Description page of Project Settings.

#include "RLPowerupBase.h"

#include "Components/SphereComponent.h"
#include "Net/UnrealNetwork.h"


ARLPowerupBase::ARLPowerupBase()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneComp = CreateDefaultSubobject<USceneComponent>("SceneComp");
	RootComponent = SceneComp;

	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetCollisionProfileName("Powerup");
	SphereComp->SetupAttachment(RootComponent);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp->SetupAttachment(SphereComp);

	RespawnTime = 10.f;
	bIsActive = true;

	//TODO: SetReplicates called on non-initialized actor. Directly setting bReplicates is the correct procedure for pre-init actors.
	SetReplicates(true);
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
	bIsActive = bNewIsActive;
	UpdateCollisionAndVisibility(bIsActive);
}

void ARLPowerupBase::UpdateCollisionAndVisibility(bool bEnable)
{
	SetActorEnableCollision(bEnable);
	RootComponent->SetVisibility(bEnable, true);
}

void ARLPowerupBase::OnRep_IsActive()
{
	UpdateCollisionAndVisibility(bIsActive);
}

void ARLPowerupBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARLPowerupBase, bIsActive);
}





