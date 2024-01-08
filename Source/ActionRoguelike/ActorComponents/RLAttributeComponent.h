// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RLAttributeComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged, AActor*, InstigatorActor, URLAttributeComponent*, OwningComponent, float, NewHalth, float, Delta);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API URLAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	URLAttributeComponent();

	UFUNCTION(BlueprintCallable, Category = "Attribute Component")
	bool ApplyHealthChange(float Delta);

	UFUNCTION(BlueprintCallable, Category = "Attribute Component")
	bool IsAlive() const;
	
public:
	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;

protected:

	// EditAnywhere - edit in BP editor and per-instance in level
	// VisibleAnywhere - read-only in editor and level (use for components)
	// EditDefaultsOnly - hide variable per-instance, edit in BP editor only
	// VisibleDefaultOnly - read-only access for variable, only in BP editor (uncommon)
	// EditInstanceOnly - allow only editing of instance (eg. when placed in level)
	// BlueprintReadOnly - read-only in BP scripting (does not effect details panel)
	// BlueprintReadWrite - read-write access in BP
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config")
	float Health;

	// HealthMax, Stamina, Strength etc...

	
};
