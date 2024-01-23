// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "RLAction.generated.h"

class URLActionComponent;

//TODO: should be abstract
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

	// Because it's UObject we should override this to have access to spawn actors, line traces etc functionality in blueprints
	virtual UWorld* GetWorld() const override;

protected:
	// just to get rid of typing GetWorld()->GetTimerManager().SetTimer/ClearTimer
	void SetWorldTimer(FTimerHandle& InOutHandle, FTimerDelegate const& InDelegate, float InRate, bool InbLoop, float InFirstDelay = -1.f);
	void ClearWorldTimer(FTimerHandle& InHandle);

	// Outer is set when creating action via NewObject<T>
	UFUNCTION(BlueprintCallable, Category = "Action")
	URLActionComponent* GetOwningComponent() const;
	
public:
	// Start immediately when added to an action component
	UPROPERTY(EditDefaultsOnly, Category = "Config | Action")
	bool bAutoStart;

	//TODO: replace this with Tag to not write FName each time
	UPROPERTY(EditDefaultsOnly, Category = "Config | Action")
	FName ActionName;

protected:
	// Tags added to Owning Actor when activated, removed when action stops
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Config | Action")
	FGameplayTagContainer GrantsTags;

	// Action can only start if Owning Actor has none of these Tags applied
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Config | Action")
	FGameplayTagContainer BlockedTags;

	bool bIsRunning;
};
