// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RLAction.h"
#include "RLAction_ProjectileAttack.generated.h"

class ARLProjectileBase;


UCLASS()
class ACTIONROGUELIKE_API URLAction_ProjectileAttack : public URLAction
{
	GENERATED_BODY()

public:
	URLAction_ProjectileAttack();
	
	//void StartAction(AActor* Instigator);

	virtual void StartAction_Implementation(AActor* Instigator) override;

protected:
	UFUNCTION()
	void AttackDelay_Elapsed(ACharacter* InstigatorCharacter);

	FVector CalculateProjectileTargetPoint(ACharacter* InstigatorCharacter);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config")
	TSubclassOf<ARLProjectileBase> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config")
	FName MuzzleSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config")
	float AttackAnimDelay;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config")
	UAnimMontage* AttackAnim;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config")
	UParticleSystem* CastingEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config")
	float TraceLength;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config")
	float TraceSphereRadius;
};
