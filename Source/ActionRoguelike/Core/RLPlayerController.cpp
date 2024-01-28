// Fill out your copyright notice in the Description page of Project Settings.

#include "RLPlayerController.h"


ARLPlayerController::ARLPlayerController()
{
}

void ARLPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	OnPawnChanged.Broadcast(InPawn);
}
