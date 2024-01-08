// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RLCharacter.generated.h"

class URLAttributeComponent;
class ARLProjectileBase;
class URLInteractionComponent;
class UCameraComponent;
class USpringArmComponent;

UCLASS()
class ACTIONROGUELIKE_API ARLCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ARLCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	
	virtual void Tick(float DeltaTime) override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
protected:
	void MoveForward(float Value);
	void MoveRight(float Value);

	void PrimaryAttack();
	void PrimaryAttack_TimeElapsed();

	void DashAttack();
	void DashAttack_TimeElapsed();

	void BlackHoleAttack();
	void BlackHoleAttack_TimeElapsed();

	void SpawnProjectile(TSubclassOf<AActor> ProjectileClassToSpawn);
	FVector CalculateProjectileTargetPoint();

	void PrimaryInteract();

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, URLAttributeComponent* OwningComp, float NewHealth, float Delta);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UCameraComponent* CameraComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	URLInteractionComponent* InteractionComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	URLAttributeComponent* AttributeComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Config | Attack | Projectile Classes")
	TSubclassOf<ARLProjectileBase> PrimaryProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Config | Attack | Projectile Classes")
	TSubclassOf<ARLProjectileBase> DashProjectileClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Config | Attack | Projectile Classes")
	TSubclassOf<ARLProjectileBase> BlackHoleProjectileClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Config | Attack | Animation Montages")
	UAnimMontage* AttackAnim;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Config | Attack | Animation Montages")
	float AttackAnimDelay;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Config | Attack | Projectile Target Point Trace")
	float TraceLength;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Config | Attack | Projectile Target Point Trace")
	float TraceSphereRadius;

	FTimerHandle TimerHandle_PrimaryAttack;
};
