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
	bInPause = false;
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

	InputComponent->BindAction("PauseMenu", IE_Pressed, this, &ARLPlayerController::TogglePauseMenu).bExecuteWhenPaused = true;
}

void ARLPlayerController::TogglePauseMenu()
{
	bInPause = !bInPause;
	
	if (!PauseMenuInstance)
	{
		CreatePauseMenu();
	}
	
	SetPause(bInPause);
	bShowMouseCursor = bInPause;
	
	if (bInPause)
	{
		FInputModeGameAndUI InputModeGameAndUI;
		InputModeGameAndUI.SetHideCursorDuringCapture(false);
		SetInputMode(InputModeGameAndUI);
		PauseMenuInstance->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		SetInputMode(FInputModeGameOnly());
		PauseMenuInstance->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void ARLPlayerController::CreatePauseMenu()
{
	PauseMenuInstance = CreateWidget<UUserWidget>(this, PauseMenuClass);
	PauseMenuInstance->AddToViewport(100);
	PauseMenuInstance->SetVisibility(ESlateVisibility::Collapsed);
}


