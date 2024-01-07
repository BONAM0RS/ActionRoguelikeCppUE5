// Fill out your copyright notice in the Description page of Project Settings.

#include "RLAttributeComponent.h"


URLAttributeComponent::URLAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	Health = 100.f;
}

bool URLAttributeComponent::ApplyHealthChange(float Delta)
{
	Health += Delta;

	OnHealthChanged.Broadcast(nullptr, this, Health, Delta);
	
	return true;
}

