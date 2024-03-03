/*
 * Third-person Action Roguelike game prototype made in Unreal Engine 5.3 as part of the Professional Game Development
 * in C++ and Unreal Engine course by Tom Looman.
 * Licensed for use only with UE based products.
 */

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "RLSaveGameSettings.generated.h"


// 'DefaultConfig' = "Save object config only to Default INIs, never to local INIs."
UCLASS(Config=Game, defaultconfig, meta = (DisplayName="Save Game Settings")) 
class ACTIONROGUELIKE_API URLSaveGameSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	URLSaveGameSettings();
	
	/* Default slot name if UI doesn't specify any */ 
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "General")
	FString SaveSlotName;
	
	/* Soft path must be converted to asset before use */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "General", AdvancedDisplay)
	TSoftObjectPtr<UDataTable> DummyTablePath;
};
