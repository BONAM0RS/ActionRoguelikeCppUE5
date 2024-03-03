/*
 * Third-person Action Roguelike game prototype made in Unreal Engine 5.3 as part of the Professional Game Development
 * in C++ and Unreal Engine course by Tom Looman.
 * Licensed for use only with UE based products.
 */

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RLExplosiveBarrel.generated.h"

class URadialForceComponent;

UCLASS()
class ACTIONROGUELIKE_API ARLExplosiveBarrel : public AActor
{
	GENERATED_BODY()
	
public:	
	ARLExplosiveBarrel();

protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

protected:
	UFUNCTION()
	void OnMeshComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		FVector NormalImpulse, const FHitResult& Hit);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	URadialForceComponent* RadialForceComponent;
};
