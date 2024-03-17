/*
 * Third-person Action Roguelike game prototype made in Unreal Engine 5.3 as part of the Professional Game Development
 * in C++ and Unreal Engine course by Tom Looman.
 * Licensed for use only with UE based products.
 */

#include "RLSaveGameSettings.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(RLSaveGameSettings)


URLSaveGameSettings::URLSaveGameSettings()
{
	// Default value while nothing is specified in the DefaultGame.ini
	SaveSlotName = "SaveGame_Default";
}
