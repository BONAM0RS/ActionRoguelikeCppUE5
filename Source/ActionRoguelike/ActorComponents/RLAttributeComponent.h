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
	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;

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

	UFUNCTION(BlueprintCallable, Category = "Attribute Component")
	float GetCurrentHealth() const;
	
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	static URLAttributeComponent* GetAttributes(AActor* FromActor);

	// DisplayName is optional, just example here
	UFUNCTION(BlueprintCallable, Category = "Attributes", meta = (DisplayName = "IsAlive"))
	static bool IsActorAlive(AActor* Actor);

protected:
	// Multicast better use for transient events, things that happened in the moment, but don't change state (like barrel explosion)
	// RepNotify are useful for state changes (for example isAlive state)
	// For example if player connected after some state changes with Multicast he will not recognize that changes

	//Reliable also ignore relevancy, so with reliable option all clients will get update (it is ok for chat msg),
	//but we maybe don't want it for players far away, in this case better use Unreliable
	
	//It can be Unreliable because MaxHealth and CurrentHealth are replicated, so its for cosmetic updates
	UFUNCTION(NetMulticast, Reliable)
	void MulticastHealthChanged(AActor* InstigatorActor, float NewHealth, float Delta);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Config")
	float MaxHealth;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Config")
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
