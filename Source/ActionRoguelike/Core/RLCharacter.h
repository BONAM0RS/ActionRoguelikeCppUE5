// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RLCharacter.generated.h"

class URLActionComponent;
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
	
protected:
	void MoveForward(float Value);
	void MoveRight(float Value);

	void SprintStart();
	void SprintStop();

	void PrimaryAttack();
	void DashAttack();
	void BlackHoleAttack();
	
	void PrimaryInteract();

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, URLAttributeComponent* OwningComp, float NewHealth, float Delta);

	void HideHitDamageEffect();

public:
	// console command, 'Exec' spicifier can be used in player controller / character / game mode / cheat manager
	UFUNCTION(Exec)
	void HealSelf(float Amount = 100.f);

	virtual FVector GetPawnViewLocation() const override;

	float GetDamageAmount();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UCameraComponent* CameraComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	URLInteractionComponent* InteractionComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	URLAttributeComponent* AttributeComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	URLActionComponent* ActionComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config")
	FName HitDamageParamName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config")
	float HideDamageHitEffectDelay;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Config")
	float DamageAmount;
	
};
