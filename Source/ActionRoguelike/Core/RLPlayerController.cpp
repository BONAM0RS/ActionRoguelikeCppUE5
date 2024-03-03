/*
 * Third-person Action Roguelike game prototype made in Unreal Engine 5.3 as part of the Professional Game Development
 * in C++ and Unreal Engine course by Tom Looman.
 * Licensed for use only with UE based products.
 */

#include "RLPlayerController.h"

#include "Blueprint/UserWidget.h"


ARLPlayerController::ARLPlayerController()
	: PauseMenuInstance(nullptr)
{
}

void ARLPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	OnPawnChanged.Broadcast(InPawn);
}

void ARLPlayerController::BeginPlayingState()
{
	Super::BeginPlayingState();

	BlueprintBeginPlayingState();
}

void ARLPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	OnPlayerStateChanged.Broadcast(PlayerState);
}

void ARLPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("PauseMenu", IE_Pressed, this, &ARLPlayerController::TogglePauseMenu);
}

void ARLPlayerController::TogglePauseMenu()
{
	// Disable Pause
	if (PauseMenuInstance != nullptr && PauseMenuInstance->IsInViewport())
	{
		PauseMenuInstance->RemoveFromParent();
		PauseMenuInstance = nullptr;

		bShowMouseCursor = false;
		SetInputMode(FInputModeGameOnly());
		return;
	}

	// Enable Pause
	PauseMenuInstance = CreateWidget<UUserWidget>(this, PauseMenuClass);
	if (PauseMenuInstance != nullptr)
	{
		PauseMenuInstance->AddToViewport(100);

		bShowMouseCursor = true;
		SetInputMode(FInputModeUIOnly());
	}
}


