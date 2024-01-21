// Fill out your copyright notice in the Description page of Project Settings.

#include "RLPlayerState.h"


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
