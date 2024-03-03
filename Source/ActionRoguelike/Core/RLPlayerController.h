/*
 * Third-person Action Roguelike game prototype made in Unreal Engine 5.3 as part of the Professional Game Development
 * in C++ and Unreal Engine course by Tom Looman.
 * Licensed for use only with UE based products.
 */

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RLPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPawnChanged, APawn*, NewPawn);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerStateChanged, APlayerState*, NewPlayerState);

UCLASS()
class ACTIONROGUELIKE_API ARLPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnPawnChanged OnPawnChanged;

	UPROPERTY(BlueprintAssignable)
	FOnPlayerStateChanged OnPlayerStateChanged;

public:
	ARLPlayerController();

protected:
	virtual void SetPawn(APawn* InPawn) override;
	
	/* Called when player controller is ready to begin playing, good moment to initialize things like Ui which might be too early in BeginPlay
		(especially in multiplayer clients where not all data such as PlayerState may have been received yet) */
	virtual void BeginPlayingState() override;

	// OPTIONS TO DEAL WITH PLAYER STATE IS NOT READY FOR UI IN MULTIPLAYER
	// Option 1, use this function to create widget instead standard BeginPlay
	UFUNCTION(BlueprintImplementableEvent)
	void BlueprintBeginPlayingState();

	// Option 2, replicatedUsing in Controller, bind to OnPlayerStateChanged in widgets
	virtual void OnRep_PlayerState() override;

protected:
	virtual void SetupInputComponent() override;
	
	UFUNCTION(BlueprintCallable)
	void TogglePauseMenu();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> PauseMenuClass;

	UPROPERTY()
	UUserWidget* PauseMenuInstance;
};
