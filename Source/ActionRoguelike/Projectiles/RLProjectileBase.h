/*
 * Third-person Action Roguelike game prototype made in Unreal Engine 5.3 as part of the Professional Game Development
 * in C++ and Unreal Engine course by Tom Looman.
 * Licensed for use only with UE based products.
 */

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RLProjectileBase.generated.h"

class UProjectileMovementComponent;
class USphereComponent;
class ULegacyCameraShake;
class USoundCue;

UCLASS(ABSTRACT)
class ACTIONROGUELIKE_API ARLProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	ARLProjectileBase();

protected:
	virtual void BeginPlay() override;

protected:
	virtual void PostInitializeComponents() override;

public:	
	virtual void Tick(float DeltaTime) override;

protected:
	UFUNCTION()
	virtual void OnSphereComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		FVector NormalImpulse, const FHitResult& Hit);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Explode();
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USphereComponent* SphereComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UParticleSystemComponent* ParticleSystemComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAudioComponent* AudioComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,  Category = "Config | Effects")
	UParticleSystem* ImpactVFX;

	UPROPERTY(EditDefaultsOnly, Category = "Config | Effects")
	USoundCue* ImpactSound;

	UPROPERTY(EditDefaultsOnly, Category = "Config | Shake")
	TSubclassOf<ULegacyCameraShake> ImpactShake;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config | Shake")
	float ImpactShakeInnerRadius;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config | Shake")
	float ImpactShakeOuterRadius;
};
