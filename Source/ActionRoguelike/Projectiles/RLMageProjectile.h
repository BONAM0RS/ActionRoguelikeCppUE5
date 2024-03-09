/*
 * Third-person Action Roguelike game prototype made in Unreal Engine 5.3 as part of the Professional Game Development
 * in C++ and Unreal Engine course by Tom Looman.
 * Licensed for use only with UE based products.
 */

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "RLProjectileBase.h"
#include "RLMageProjectile.generated.h"


class URLAction_Effect;

UCLASS(Abstract)
class ACTIONROGUELIKE_API ARLMageProjectile : public ARLProjectileBase
{
	GENERATED_BODY()
	
public:
	ARLMageProjectile();

protected:
	virtual void PostInitializeComponents() override;
  
	UFUNCTION()
	void OnSphereComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	virtual void OnSphereComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		FVector NormalImpulse, const FHitResult& Hit);

public:
	void SetDamageAmount(float NewDamageAmount);

protected:
	// ExposeOnSpawn allow to set property in spawn actor from class node
	// TODO: maybe hide it from editing in default, just expose it
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ExposeOnSpawn="true"), Category = "Config")
	float DamageAmount;

	UPROPERTY(EditDefaultsOnly, Category = "Config")
	FGameplayTag ParryTag;

	UPROPERTY(EditDefaultsOnly, Category = "Config")
	TSubclassOf<URLAction_Effect> BurningActionEffectClass;

	UPROPERTY(EditDefaultsOnly, Category = "Config")
	TSubclassOf<URLAction_Effect> StunningActionEffectClass;
};
