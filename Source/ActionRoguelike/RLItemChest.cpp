// Fill out your copyright notice in the Description page of Project Settings.

#include "RLItemChest.h"


ARLItemChest::ARLItemChest()
{
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	RootComponent = BaseMesh;

	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>("LidMesh");
	LidMesh->SetupAttachment(BaseMesh);

	TargetPitch = 110.f;
}

void ARLItemChest::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARLItemChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARLItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	LidMesh->SetRelativeRotation(FRotator(TargetPitch, 0.f, 0.f));
}
