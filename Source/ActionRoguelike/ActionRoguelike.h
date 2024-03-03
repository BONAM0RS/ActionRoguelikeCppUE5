// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

// Added here to more easily include whenever we also use LogGame (structuredlog is new in 5.2)
#include "Logging/StructuredLog.h"

// Define category "LogGame"
ACTIONROGUELIKE_API DECLARE_LOG_CATEGORY_EXTERN(LogGame, Log, All);

// Define new "stat command" group shown in-game 
DECLARE_STATS_GROUP(TEXT("ANDREW_Game"), STATGROUP_ANDREW, STATCAT_Advanced);

// Define alias to easily keep track of custom channels in C++ (must match what is specified in Project Settings > Collision 
#define COLLISION_PROJECTILE ECC_GameTraceChannel1


static void LogOnScreen(UObject* WorldContext, FString Msg, FColor Color = FColor::White, float Duration = 5.f)
{
	if (!ensure(WorldContext))
	{
		return;
	}

	UWorld* World = WorldContext->GetWorld();
	if (!ensure(World))
	{
		return;
	}

	FString NetPrefix = World->IsNetMode(NM_Client) ? "[CLIENT] " : "[SERVER] ";
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, Duration, Color, NetPrefix + Msg);
	}
}

