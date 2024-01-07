// Fill out your copyright notice in the Description page of Project Settings.

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
