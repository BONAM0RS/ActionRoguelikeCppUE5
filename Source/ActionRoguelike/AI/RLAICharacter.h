// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RLAICharacter.generated.h"

class URLActionComponent;
class URLWorldUserWidget;
class URLAttributeComponent;
class UPawnSensingComponent;

UCLASS()
class ACTIONROGUELIKE_API ARLAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ARLAICharacter();

protected:
	virtual void PostInitializeComponents() override;
	
	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, URLAttributeComponent* OwningComp, float NewHealth, float Delta);
	
	void SetTargetActor(AActor* NewTarget);
	
	void HideHitDamageEffect();
	
public:
	UFUNCTION(BlueprintCallable, Category = "AI")
	AActor* GetTargetActor() const;
	
	float GetDamageAmount();
	
	FName GetMuzzleShotSocketName();

protected:
	UPROPERTY(VisibleAnywhere)
	UPawnSensingComponent* PawnSensingComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	URLAttributeComponent* AttributeComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	URLActionComponent* ActionComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config | Names")
	FName MuzzleShotSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config | Names")
	FName HitDamageParamName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Config | Damage")
	float DamageAmount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config | Effects")
	float HideDamageHitEffectDelay;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config | UI")
	TSubclassOf<UUserWidget> HealthBarWidgetClass;

	UPROPERTY()
	URLWorldUserWidget* ActiveHealthBarWidget;

	/* Key for AI Blackboard 'TargetActor' */
	UPROPERTY(VisibleAnywhere, Category = "Config | Names")
	FName TargetActorKey;

	/* Widget to display when bot first sees a player. */
	UPROPERTY(EditDefaultsOnly, Category = "Config | UI")
	TSubclassOf<UUserWidget> SpottedWidgetClass;
};
