/*
 * Third-person Action Roguelike game prototype made in Unreal Engine 5.3 as part of the Professional Game Development
 * in C++ and Unreal Engine course by Tom Looman.
 * Licensed for use only with UE based products.
 */

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RLAttributeComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnAttributeChanged, AActor*, InstigatorActor, URLAttributeComponent*, OwningComponent, float, NewValue, float, Delta);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API URLAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = "Attribute Component")
	FOnAttributeChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "Attribute Component")
	FOnAttributeChanged OnRageChanged;

public:	
	URLAttributeComponent();

	UFUNCTION(BlueprintCallable, Category = "Attribute Component")
	bool ApplyHealthChange(AActor* InstigatorActor, float Delta);

	UFUNCTION(BlueprintCallable, Category = "Attribute Component")
	bool ApplyRageChange(AActor* InstigatorActor, float Delta);

protected:
	/* Multicast better use for transient events, things that happened in the moment, but don't change state (like barrel explosion)
		RepNotify are useful for state changes (for example isAlive state)
		For example if player connected after some state changes with Multicast he will not recognize that changes */

	/* Reliable also ignore relevancy, so with reliable option all clients will get update (it is ok for chat msg),
		but we maybe don't want it for players far away, in this case better use Unreliable */
	
	// It can be Unreliable because MaxHealth and CurrentHealth are replicated, so its for cosmetic updates
	UFUNCTION(NetMulticast, Reliable)
	void MulticastHealthChanged(AActor* InstigatorActor, float NewHealth, float Delta);

	// Used for cosmetic changes only
	UFUNCTION(NetMulticast, Unreliable) 
	void MulticastRageChanged(AActor* InstigatorActor, float NewRage, float Delta);

public:
	UFUNCTION(BlueprintCallable, Category = "Attribute Component")
	bool Kill(AActor* InstigatorActor);

	UFUNCTION(BlueprintCallable, Category = "Attribute Component")
	bool IsAlive() const;

	UFUNCTION(BlueprintCallable, Category = "Attribute Component")
	bool IsFullHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Attribute Component")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Attribute Component")
	float GetCurrentHealth() const;
	
	UFUNCTION(BlueprintCallable, Category = "Attribute Component")
	float GetCurrentRage() const;
	
	UFUNCTION(BlueprintCallable, Category = "Attribute Component")
	static URLAttributeComponent* GetAttributes(AActor* FromActor);

	// DisplayName is optional, just example here
	UFUNCTION(BlueprintCallable, Category = "Attribute Component", meta = (DisplayName = "IsAlive"))
	static bool IsActorAlive(AActor* Actor);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Config")
	float MaxHealth;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Config")
	float CurrentHealth;

	// Resource used to power certain Actions
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Config")
	float MaxRage;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Config")
	float CurrentRage;
	
	// Stamina, Strength etc...

	/*
	EditAnywhere - edit in BP editor and per-instance in level
	VisibleAnywhere - read-only in editor and level (use for components)
	EditDefaultsOnly - hide variable per-instance, edit in BP editor only
	VisibleDefaultOnly - read-only access for variable, only in BP editor (uncommon)
	EditInstanceOnly - allow only editing of instance (eg. when placed in level)
	BlueprintReadOnly - read-only in BP scripting (does not effect details panel)
	BlueprintReadWrite - read-write access in BP
	*/
};
