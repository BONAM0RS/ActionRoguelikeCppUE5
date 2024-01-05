// Fill out your copyright notice in the Description page of Project Settings.

#include "RLExplosiveBarrel.h"

#include "PhysicsEngine/RadialForceComponent.h"


ARLExplosiveBarrel::ARLExplosiveBarrel()
{
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	StaticMeshComponent->SetSimulatePhysics(true);
	StaticMeshComponent->SetCollisionProfileName("PhysicsActor");
	RootComponent = StaticMeshComponent;
	
	RadialForceComponent = CreateDefaultSubobject<URadialForceComponent>("RadialForceComponent");
	RadialForceComponent->SetupAttachment(StaticMeshComponent);
}

void ARLExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();

	StaticMeshComponent->OnComponentHit.AddDynamic(this, &ARLExplosiveBarrel::OnHit);
	UE_LOG(LogTemp, Log, TEXT("%S"), __FUNCTION__);
}

void ARLExplosiveBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARLExplosiveBarrel::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	RadialForceComponent->FireImpulse();
	UE_LOG(LogTemp, Log, TEXT("%S"), __FUNCTION__);
}

