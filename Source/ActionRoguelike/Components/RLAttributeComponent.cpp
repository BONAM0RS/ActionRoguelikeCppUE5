/*
 * Third-person Action Roguelike game prototype made in Unreal Engine 5.3 as part of the Professional Game Development
 * in C++ and Unreal Engine course by Tom Looman.
 * Licensed for use only with UE based products.
 */

#include "RLAttributeComponent.h"

#include "ActionRoguelike/Core/RLGameModeBase.h"
#include "Net/UnrealNetwork.h"


static TAutoConsoleVariable<float> CVarDamageMultiplier(TEXT("su.DamageMultiplier"), 1.0f,
                                                        TEXT("Global Damage Modifier for Attribute Component"), ECVF_Cheat);


URLAttributeComponent::URLAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	MaxHealth = 100.f;
	CurrentHealth = MaxHealth;

	MaxRage = 100.f;
	CurrentRage = 0.f;

	// We should use SetIsReplicatedByDefault for components instead SetReplicates because of another init flow in actor component constructor
	SetIsReplicatedByDefault(true);
}

bool URLAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	if (Delta < 0.0f)
	{
		// Just to not damage if god mode is enabled using cheats
		if (!GetOwner()->CanBeDamaged()) {
			return false;
		}

		// Apply damage multiplier for test purposes
		float DamageMultiplier = CVarDamageMultiplier.GetValueOnGameThread();
		Delta *= DamageMultiplier;
	}
	
	float OldCurrentHealth = CurrentHealth;
	float NewCurrentHealth = FMath::Clamp(CurrentHealth + Delta, 0.0f, MaxHealth);
	float RealDelta = NewCurrentHealth - OldCurrentHealth;

	// Is Server?
	if (GetOwner()->HasAuthority())
	{
		CurrentHealth = NewCurrentHealth;

		if (RealDelta != 0.0f)
		{
			MulticastHealthChanged(InstigatorActor, CurrentHealth, RealDelta);
		}

		// Died
		if (RealDelta < 0.0f && CurrentHealth == 0.0f)
		{
			ARLGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ARLGameModeBase>();
			if (GameMode != nullptr) {
				GameMode->OnActorKilled(GetOwner(), InstigatorActor);
			}
		}
	}
	
	return RealDelta != 0.0f;
}

bool URLAttributeComponent::ApplyRageChange(AActor* InstigatorActor, float Delta)
{
	float OldCurrentRage = CurrentRage;
	CurrentRage = FMath::Clamp(CurrentRage + Delta, 0.0f, MaxRage);
	float RealDelta = CurrentRage - OldCurrentRage;

	if (RealDelta != 0.0f)
	{
		OnRageChanged.Broadcast(InstigatorActor, this, CurrentRage, RealDelta);
	}
	
	return RealDelta != 0.0f;
}

void URLAttributeComponent::MulticastHealthChanged_Implementation(AActor* InstigatorActor, float NewHealth, float Delta)
{
	OnHealthChanged.Broadcast(InstigatorActor, this, NewHealth, Delta);
}

void URLAttributeComponent::MulticastRageChanged_Implementation(AActor* InstigatorActor, float NewRage, float Delta)
{
	OnRageChanged.Broadcast(InstigatorActor, this, NewRage, Delta);
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

float URLAttributeComponent::GetCurrentRage() const
{
	return CurrentRage;
}

URLAttributeComponent* URLAttributeComponent::GetAttributes(AActor* FromActor)
{
	if (FromActor != nullptr) {
		return Cast<URLAttributeComponent>(FromActor->GetComponentByClass(URLAttributeComponent::StaticClass()));
	}

	return nullptr;
}

bool URLAttributeComponent::IsActorAlive(AActor* Actor)
{
	URLAttributeComponent* AttributeComp = GetAttributes(Actor);
	if (AttributeComp != nullptr) {
		return AttributeComp->IsAlive();
	}

	return false;
}

void URLAttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(URLAttributeComponent, MaxHealth);
	DOREPLIFETIME(URLAttributeComponent, CurrentHealth);

	DOREPLIFETIME(URLAttributeComponent, MaxRage);
	DOREPLIFETIME(URLAttributeComponent, CurrentRage);
	
}


