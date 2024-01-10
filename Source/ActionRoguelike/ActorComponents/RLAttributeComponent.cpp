// Fill out your copyright notice in the Description page of Project Settings.

#include "RLAttributeComponent.h"


URLAttributeComponent::URLAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	MaxHealth = 100.f;
	CurrentHealth = MaxHealth;
}

bool URLAttributeComponent::ApplyHealthChange(float Delta)
{
	float OldCurrentHealth = CurrentHealth;
	CurrentHealth = FMath::Clamp(CurrentHealth + Delta, 0.0f, MaxHealth);
	float RealDelta = CurrentHealth - OldCurrentHealth;

	OnHealthChanged.Broadcast(nullptr, this, CurrentHealth, RealDelta);
	
	return RealDelta != 0.0f;
}

bool URLAttributeComponent::IsAlive() const
{
	return CurrentHealth > 0.0f;
}

bool URLAttributeComponent::IsFullHealth() const
{
	return CurrentHealth == MaxHealth;
}

float URLAttributeComponent::GetMaxHealth() const
{
	return MaxHealth;
}



