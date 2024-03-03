// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "RLSaveGameSettings.generated.h"


UCLASS(Config=Game, defaultconfig, meta = (DisplayName="Save Game Settings")) // 'DefaultConfig' = "Save object config only to Default INIs, never to local INIs."
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
