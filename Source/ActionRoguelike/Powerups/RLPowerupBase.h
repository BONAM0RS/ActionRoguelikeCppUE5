/*
 * Third-person Action Roguelike game prototype made in Unreal Engine 5.3 as part of the Professional Game Development
 * in C++ and Unreal Engine course by Tom Looman.
 * Licensed for use only with UE based products.
 */

#pragma once

#include "CoreMinimal.h"
#include "ActionRoguelike/Interfaces/RLGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "RLPowerupBase.generated.h"

class USphereComponent;

UCLASS(Abstract)
class ACTIONROGUELIKE_API ARLPowerupBase : public AActor, public IRLGameplayInterface
{
	GENERATED_BODY()
	
public:	
	ARLPowerupBase();
	
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

	virtual FText GetInteractText_Implementation(APawn* InstigatorPawn) override;

protected:
	UFUNCTION()
	void ShowPowerup();
	
	void HideAndCooldownPowerup();

	void SetPowerupState(bool bNewIsActive);

	void UpdateCollisionAndVisibility(bool bEnable);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* SceneComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* SphereComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere, Category = "Config")
	float RespawnTime;

	FTimerHandle TimerHandle_RespawnTimer;

	UPROPERTY(ReplicatedUsing="OnRep_IsActive")
	bool bIsActive;

	UFUNCTION()
	void OnRep_IsActive();
};
