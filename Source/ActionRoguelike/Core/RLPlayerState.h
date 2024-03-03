/*
 * Third-person Action Roguelike game prototype made in Unreal Engine 5.3 as part of the Professional Game Development
 * in C++ and Unreal Engine course by Tom Looman.
 * Licensed for use only with UE based products.
 */

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "RLPlayerState.generated.h"

class URLSaveGame;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnRecordTimeChanged, ARLPlayerState*, PlayerState, float, NewTime, float, OldRecord);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCreditsChanged, ARLPlayerState*, PlayerState, int32, NewCredits,
                                               int32, Delta);

UCLASS()
class ACTIONROGUELIKE_API ARLPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnRecordTimeChanged OnRecordTimeChanged;
	
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnCreditsChanged OnCreditsChanged;

public:
	UFUNCTION(BlueprintNativeEvent)
	void SavePlayerState(URLSaveGame* SaveGameObject);

	UFUNCTION(BlueprintNativeEvent)
	void LoadPlayerState(URLSaveGame* SaveGameObject);

public:
	// Checks current record and only sets if better time was passed in
	UFUNCTION(BlueprintCallable)
	bool UpdatePersonalRecord(float NewTime);
	
	UFUNCTION(BlueprintCallable, Category = "Credits")
	void AddCredits(int32 Delta);

	UFUNCTION(BlueprintCallable, Category = "Credits")
	bool RemoveCredits(int32 Delta);

	UFUNCTION(BlueprintCallable, Category = "Credits")
	int32 GetCredits() const;

protected:
	UPROPERTY(BlueprintReadOnly)
	float PersonalRecordTime;
	
	UPROPERTY(EditDefaultsOnly, ReplicatedUsing="OnRep_Credits", Category = "Config")
	int32 Credits;

	// OnRep_ can use a parameter containing the 'old value' of the variable it is bound to. Very useful in this case to figure out the 'delta'
	UFUNCTION()
	void OnRep_Credits(int32 OldCredits);

	/* Downside of using multicast here is that we send over more data the net, since it's a RPC with two params.
		OnRep_ is "free" since Credits is already getting replicated anyway */
	//UFUNCTION(NetMulticast, Unreliable);
	//void MulticastCredits(float NewCredits, float Delta);
};
