/*
 * Third-person Action Roguelike game prototype made in Unreal Engine 5.3 as part of the Professional Game Development
 * in C++ and Unreal Engine course by Tom Looman.
 * Licensed for use only with UE based products.
 */

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RLDummyTarget.generated.h"

class URLAttributeComponent;

UCLASS()
class ACTIONROGUELIKE_API ARLDummyTarget : public AActor
{
	GENERATED_BODY()
	
public:	
	ARLDummyTarget();

protected:
	virtual void PostInitializeComponents() override;

protected:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComp;
	
	UPROPERTY(VisibleAnywhere)
	URLAttributeComponent* AttributeComp;

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, URLAttributeComponent* OwningComp, float NewHealth, float Delta);
};
