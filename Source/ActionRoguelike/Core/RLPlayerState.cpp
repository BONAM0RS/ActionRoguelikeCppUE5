// Fill out your copyright notice in the Description page of Project Settings.

#include "RLPlayerState.h"

#include "ActionRoguelike/SaveGame/RLSaveGame.h"
#include "Net/UnrealNetwork.h"


void ARLPlayerState::SavePlayerState_Implementation(URLSaveGame* SaveGameObject)
{
	if (SaveGameObject != nullptr)
	{
		SaveGameObject->Credits = Credits;
	}
}

void ARLPlayerState::LoadPlayerState_Implementation(URLSaveGame* SaveGameObject)
{
	if (SaveGameObject != nullptr)
	{
		AddCredits(SaveGameObject->Credits);
	}
}

void ARLPlayerState::AddCredits(int32 Delta)
{
	if (!ensure(Delta > 0.0f)) {
		return;
	}

	Credits += Delta;
	OnCreditsChanged.Broadcast(this, Credits, Delta);
}

bool ARLPlayerState::RemoveCredits(int32 Delta)
{
	if (!ensure(Delta > 0.0f)) {
		return false;
	}

	if (Credits < Delta) {
		return false;
	}

	Credits -= Delta;
	OnCreditsChanged.Broadcast(this, Credits, Delta);
	
	return true;
}

int32 ARLPlayerState::GetCredits() const
{
	return Credits;
}

void ARLPlayerState::OnRep_Credits(int32 OldCredits)
{
	OnCreditsChanged.Broadcast(this, Credits, Credits - OldCredits);
}

void ARLPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARLPlayerState, Credits);
}
