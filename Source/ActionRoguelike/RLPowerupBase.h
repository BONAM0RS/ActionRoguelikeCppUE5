// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/RLGameplayInterface.h"
#include "RLPowerupBase.generated.h"

class USphereComponent;

UCLASS(Abstract)
class ACTIONROGUELIKE_API ARLPowerupBase : public AActor, public IRLGameplayInterface
{
	GENERATED_BODY()
	
public:	
	ARLPowerupBase();
	
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

protected:
	UFUNCTION()
	void ShowPowerup();
	
	void HideAndCooldownPowerup();

	void SetPowerupState(bool bNewIsActive);

	void UpdateCollisionAndVisibility(bool bEnable);

protected:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USceneComponent* SceneComp;
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* SphereComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere, Category = "Config")
	float RespawnTime;

	FTimerHandle TimerHandle_RespawnTimer;

	UPROPERTY(ReplicatedUsing="OnRep_IsActive")
	bool bIsActive;

	UFUNCTION()
	void OnRep_IsActive();
};
