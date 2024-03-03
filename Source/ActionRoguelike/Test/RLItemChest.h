// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionRoguelike/Interfaces/RLGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "RLItemChest.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ARLItemChest : public AActor, public IRLGameplayInterface
{
	GENERATED_BODY()
	
public:	
	ARLItemChest();

	void OnActorLoaded_Implementation() override;

public:	
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

protected:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* LidMesh;

	UPROPERTY(EditAnywhere);
	float TargetPitch;
	
protected:
	void InteractWithLid();

protected:
	UFUNCTION()
	void OnRep_LidOpened();
	
protected:
	// ReplicatedUsing = RepNotify, but it calls function only for clients unlike RepNotify in BP!
	UPROPERTY(ReplicatedUsing = "OnRep_LidOpened", BlueprintReadOnly, SaveGame)
	bool bLidOpened;
	
};
