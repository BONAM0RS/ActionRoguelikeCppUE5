// Fill out your copyright notice in the Description page of Project Settings.

#include "RLAttributeComponent.h"

#include "ActionRoguelike/Core/RLGameModeBase.h"


static TAutoConsoleVariable<float> CVarDamageMultiplier(TEXT("su.DamageMultiplier"), 1.0f,
	TEXT("Global Damage Modifier for Attribute Component"), ECVF_Cheat);


URLAttributeComponent::URLAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	MaxHealth = 100.f;
	CurrentHealth = MaxHealth;
}

bool URLAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	if (Delta < 0.0f)
	{
		// just to not damage if god mode is enabled using cheats
		if (!GetOwner()->CanBeDamaged()) {
			return false;
		}
		
		float DamageMultiplier = CVarDamageMultiplier.GetValueOnGameThread();
		Delta *= DamageMultiplier;
	}
	
	float OldCurrentHealth = CurrentHealth;
	CurrentHealth = FMath::Clamp(CurrentHealth + Delta, 0.0f, MaxHealth);
	float RealDelta = CurrentHealth - OldCurrentHealth;

	OnHealthChanged.Broadcast(InstigatorActor, this, CurrentHealth, RealDelta);

	// Died
	if (RealDelta < 0.0f && CurrentHealth == 0.0f)
	{
		ARLGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ARLGameModeBase>();
		if (GameMode != nullptr) {
			GameMode->OnActorKilled(GetOwner(), InstigatorActor);
		}
	}
	
	return RealDelta != 0.0f;
}

bool URLAttributeComponent::Kill(AActor* InstigatorActor)
{
	return ApplyHealthChange(InstigatorActor, -MaxHealth);
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

float URLAttributeComponent::GetCurrentHealth() const
{
	return CurrentHealth;
}

bool URLAttributeComponent::IsActorAlive(AActor* Actor)
{
	URLAttributeComponent* AttributeComp = GetAttributes(Actor);
	if (AttributeComp != nullptr) {
		return AttributeComp->IsAlive();
	}

	return false;
}

URLAttributeComponent* URLAttributeComponent::GetAttributes(AActor* FromActor)
{
	if (FromActor != nullptr) {
		return Cast<URLAttributeComponent>(FromActor->GetComponentByClass(URLAttributeComponent::StaticClass()));
	}

	return nullptr;
}



