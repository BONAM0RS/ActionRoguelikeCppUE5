// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/RLGameplayInterface.h"
#include "RLItemChest.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ARLItemChest : public AActor, public IRLGameplayInterface
{
	GENERATED_BODY()
	
public:	
	ARLItemChest();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

protected:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* LidMesh;

	UPROPERTY(EditAnywhere);
	float TargetPitch;
};
