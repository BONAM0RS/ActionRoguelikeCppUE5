// Fill out your copyright notice in the Description page of Project Settings.

#include "RLItemChest.h"

#include "Net/UnrealNetwork.h"


ARLItemChest::ARLItemChest()
{
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	RootComponent = BaseMesh;

	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>("LidMesh");
	LidMesh->SetupAttachment(BaseMesh);

	TargetPitch = 110.f;

	//TODO: SetReplicates called on non-initialized actor. Directly setting bReplicates is the correct procedure for pre-init actors.
	SetReplicates(true);
}

void ARLItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	// toggle
	bLidOpened = !bLidOpened;

	//ReplicatedUsing function is called only on clients, so we need to call it's on server manually
	InteractWithLid();
}

void ARLItemChest::InteractWithLid()
{
	float CurrentPitch = bLidOpened ? TargetPitch : 0.f;
	LidMesh->SetRelativeRotation(FRotator(CurrentPitch, 0.f, 0.f));
}

void ARLItemChest::OnRep_LidOpened()
{
	InteractWithLid();
}

// you need just write implementation if this function to replicate property if any has UPROPERTY(Replicated/ReplicatedUsing)
void ARLItemChest::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// send update of this property to all clients from server
	DOREPLIFETIME(ARLItemChest, bLidOpened);
}

