/*
 * Third-person Action Roguelike game prototype made in Unreal Engine 5.3 as part of the Professional Game Development
 * in C++ and Unreal Engine course by Tom Looman.
 * Licensed for use only with UE based products.
 */


#include "RLSaveGame.h"

#include "ActionRoguelike/ActionRoguelike.h"
#include "GameFramework/PlayerState.h"

FPlayerSaveData* URLSaveGame::GetPlayerData(APlayerState* PlayerState)
{
	if (PlayerState == nullptr)
	{
		return nullptr;
	}

	/* Will not give unique ID while PIE so we skip that step while testing in editor.
		UObjects don't have access to UWorld, so we grab it via PlayerState instead */
	if (PlayerState->GetWorld()->IsPlayInEditor())
	{
		UE_LOGFMT(LogGame, Log, "During PIE we cannot use PlayerID to retrieve Saved Player data. Using first entry in array if available.");

		if (SavedPlayers.IsValidIndex(0))
		{
			return &SavedPlayers[0];
		}

		// No saved player data available
		return nullptr;
	}

	/* Easiest way to deal with the different IDs is as FString (original Steam id is uint64)
		Keep in mind that GetUniqueId() returns the online id, where GetUniqueID() is a function from UObject (very confusing...) */
	FString PlayerID = PlayerState->GetUniqueId().ToString();
	
	// Iterate the array and match by PlayerID (eg. unique ID provided by Steam)
	return SavedPlayers.FindByPredicate([&](const FPlayerSaveData& Data) { return Data.PlayerID == PlayerID; });
}
