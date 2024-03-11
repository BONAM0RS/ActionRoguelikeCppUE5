/*
 * Third-person Action Roguelike game prototype made in Unreal Engine 5.3 as part of the Professional Game Development
 * in C++ and Unreal Engine course by Tom Looman.
 * Licensed for use only with UE based products.
 */

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
	
	bReplicates = true;
}

void ARLPowerupBase::Interact_Implementation(APawn* InstigatorPawn)
{
	// override in derived classes
}

FText ARLPowerupBase::GetInteractText_Implementation(APawn* InstigatorPawn)
{
	//return FText::GetEmpty();
	FText DefaultText = FText::FromString(TEXT("Default Interaction Message")); 
	return DefaultText;
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





