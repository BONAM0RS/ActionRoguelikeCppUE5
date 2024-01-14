// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RLAttributeComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged, AActor*, InstigatorActor, URLAttributeComponent*, OwningComponent, float, NewHealth, float, Delta);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API URLAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	URLAttributeComponent();

	UFUNCTION(BlueprintCallable, Category = "Attribute Component")
	bool ApplyHealthChange(AActor* InstigatorActor, float Delta);

	UFUNCTION(BlueprintCallable)
	bool Kill(AActor* InstigatorActor);

	UFUNCTION(BlueprintCallable, Category = "Attribute Component")
	bool IsAlive() const;

	UFUNCTION(BlueprintCallable, Category = "Attribute Component")
	bool IsFullHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Attribute Component")
	float GetMaxHealth() const;
	
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	static URLAttributeComponent* GetAttributes(AActor* FromActor);

	// DisplayName is optional, just example here
	UFUNCTION(BlueprintCallable, Category = "Attributes", meta = (DisplayName = "IsAlive"))
	static bool IsActorAlive(AActor* Actor);
	
public:
	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config")
	float MaxHealth;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config")
	float CurrentHealth;
	
	// Stamina, Strength etc...
	
	// EditAnywhere - edit in BP editor and per-instance in level
	// VisibleAnywhere - read-only in editor and level (use for components)
	// EditDefaultsOnly - hide variable per-instance, edit in BP editor only
	// VisibleDefaultOnly - read-only access for variable, only in BP editor (uncommon)
	// EditInstanceOnly - allow only editing of instance (eg. when placed in level)
	// BlueprintReadOnly - read-only in BP scripting (does not effect details panel)
	// BlueprintReadWrite - read-write access in BP
};
