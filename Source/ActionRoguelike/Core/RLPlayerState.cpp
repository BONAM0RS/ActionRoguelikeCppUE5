/*
 * Third-person Action Roguelike game prototype made in Unreal Engine 5.3 as part of the Professional Game Development
 * in C++ and Unreal Engine course by Tom Looman.
 * Licensed for use only with UE based products.
 */

#include "RLPlayerState.h"

#include "Net/UnrealNetwork.h"
#include "ActionRoguelike/ActionRoguelike.h"
#include "ActionRoguelike/Save/RLSaveGame.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(RLPlayerState)


void ARLPlayerState::SavePlayerState_Implementation(URLSaveGame* SaveGameObject)
{
	if (SaveGameObject != nullptr)
	{
		// Gather all relevant data for player
		FPlayerSaveData SaveData;
		SaveData.Credits = Credits;
		// Stored as FString for simplicity (original Steam ID is uint64)
		SaveData.PlayerID = GetUniqueId().ToString();

		// May not be alive while we save
		if (APawn* MyPawn = GetPawn())
		{
			SaveData.Location = MyPawn->GetActorLocation();
			SaveData.Rotation = MyPawn->GetActorRotation();
			SaveData.bResumeAtTransform = true;
		}
		
		SaveGameObject->SavedPlayers.Add(SaveData);
	}
}

void ARLPlayerState::LoadPlayerState_Implementation(URLSaveGame* SaveGameObject)
{
	if (SaveGameObject != nullptr)
	{
		FPlayerSaveData* FoundData = SaveGameObject->GetPlayerData(this);
		if (FoundData)
		{
			// Makes sure we trigger credits changed event
			AddCredits(FoundData->Credits);
		}
		else
		{
			UE_LOGFMT(LogGame, Warning, "Could not find SaveGame data for player id: {playerid}.", GetPlayerId());
		}
	}
}

void ARLPlayerState::AddCredits(int32 Delta)
{
	if (!ensure(Delta >= 0.0f)) {
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
