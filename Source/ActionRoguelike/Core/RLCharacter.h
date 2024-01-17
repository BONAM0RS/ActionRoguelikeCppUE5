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

// better have child classes from this base class: RLPlayerCharacter and RLAICharacter
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

public:
	// console command, 'Exec' spicifier can be used in player controller / character / game mode / cheat manager
	UFUNCTION(Exec)
	void HealSelf(float Amount = 100.f);

	virtual FVector GetPawnViewLocation() const override;
	
protected:
	void MoveForward(float Value);
	void MoveRight(float Value);

	void PrimaryAttack();
	void PrimaryAttack_TimeElapsed();

	void DashAttack();
	void DashAttack_TimeElapsed();

	void BlackHoleAttack();
	void BlackHoleAttack_TimeElapsed();

	void StartAttackEffect();

	void SpawnProjectile(TSubclassOf<AActor> ProjectileClassToSpawn);
	FVector CalculateProjectileTargetPoint();

	void PrimaryInteract();

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, URLAttributeComponent* OwningComp, float NewHealth, float Delta);

	void HideHitDamageEffect();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UCameraComponent* CameraComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	URLInteractionComponent* InteractionComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	URLAttributeComponent* AttributeComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config | Names")
	FName PrimaryHandSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config | Names")
	FName HitDamageParamName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Config | Damage")
	float DamageAmount;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config | Projectile Classes")
	TSubclassOf<ARLProjectileBase> PrimaryProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config | Projectile Classes")
	TSubclassOf<ARLProjectileBase> DashProjectileClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config | Projectile Classes")
	TSubclassOf<ARLProjectileBase> BlackHoleProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config | Effects")
	UParticleSystem* CastingEffect;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config | Animation Montages")
	UAnimMontage* AttackAnim;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config | Animation Montages")
	float AttackAnimDelay;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config | Effects")
	float HideDamageHitEffectDelay;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config | Projectile Target Point Trace")
	float TraceLength;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config | Projectile Target Point Trace")
	float TraceSphereRadius;
	
	FTimerHandle TimerHandle_PrimaryAttack;
	
};
