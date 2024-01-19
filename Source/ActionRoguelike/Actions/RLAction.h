// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "RLAction.generated.h"

class URLActionComponent;

// should be abstract
UCLASS(Blueprintable)
class ACTIONROGUELIKE_API URLAction : public UObject
{
	GENERATED_BODY()

public:
	URLAction();
	
	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StartAction(AActor* Instigator);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Action")
	void StopAction(AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	bool CanStart(AActor* Instigator);

	UFUNCTION(BlueprintCallable, Category = "Action")
	bool IsRunning() const;

	virtual UWorld* GetWorld() const override;

protected:
	UFUNCTION(BlueprintCallable, Category = "Action")
	URLActionComponent* GetOwningComponent() const;
	
public:
	// replace this with Tag to not write FName each time
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	FName ActionName;

protected:
	// Tags added to Owning Actor when activated, removed when action stops
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Tags")
	FGameplayTagContainer GrantsTags;

	// Action can only start if Owning Actor has none of these Tags applied
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Tags")
	FGameplayTagContainer BlockedTags;

	bool bIsRunning;
};
