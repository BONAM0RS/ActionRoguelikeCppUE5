/*
 * Third-person Action Roguelike game prototype made in Unreal Engine 5.3 as part of the Professional Game Development
 * in C++ and Unreal Engine course by Tom Looman.
 * Licensed for use only with UE based products.
 */

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RLCharacter.generated.h"

class URLActionComponent;
class URLAttributeComponent;
class ARLProjectileBase;
class URLInteractionComponent;
class UCameraComponent;
class USpringArmComponent;


// TODO: Better to have child classes from this base class: RLPlayerCharacter and RLAICharacter
UCLASS()
class ACTIONROGUELIKE_API ARLCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ARLCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	
	virtual void Tick(float DeltaTime) override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
protected:
	void MoveForward(float Value);
	void MoveRight(float Value);
	
	void SprintStop();
	
	void PrimaryInteract();

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, URLAttributeComponent* OwningComp, float NewHealth, float Delta);

	UFUNCTION()
	void OnDiedCapsuleHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

	void HideHitDamageEffect();

public:
	// Console command, 'Exec' spicifier can be used in player controller / character / game mode / cheat manager
	UFUNCTION(Exec)
	void HealSelf(float Amount = 100.f);

	virtual FVector GetPawnViewLocation() const override;

	float GetDamageAmount();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UCameraComponent* CameraComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	URLInteractionComponent* InteractionComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	URLAttributeComponent* AttributeComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	URLActionComponent* ActionComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config")
	FName HitDamageParamName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config")
	float HideDamageHitEffectDelay;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Config")
	float DamageAmount;
	
};
