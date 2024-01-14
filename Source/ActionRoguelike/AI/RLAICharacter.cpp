// Fill out your copyright notice in the Description page of Project Settings.

#include "RLAICharacter.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "ActionRoguelike/ActorComponents/RLAttributeComponent.h"
#include "ActionRoguelike/UI/RLWorldUserWidget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/UserWidget.h"
#include "Perception/PawnSensingComponent.h"


ARLAICharacter::ARLAICharacter()
{
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");
	AttributeComponent = CreateDefaultSubobject<URLAttributeComponent>("AttributeComp");

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	MuzzleShotSocketName = "Muzzle_Front";
	HitDamageParamName = "Damage";

	HideDamageHitEffectDelay = 0.2f;

	DamageAmount = 5.f;

	ActiveHealthBarWidget = nullptr;
}

void ARLAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	PawnSensingComp->OnSeePawn.AddDynamic(this, &ARLAICharacter::OnPawnSeen);
	AttributeComponent->OnHealthChanged.AddDynamic(this, &ARLAICharacter::OnHealthChanged);
}

void ARLAICharacter::OnPawnSeen(APawn* Pawn)
{
	SetTargetActor(Pawn);
	
	DrawDebugString(GetWorld(), GetActorLocation(), "PLAYER SPOTTED", nullptr, FColor::White, 4.0f, true);
}

void ARLAICharacter::SetTargetActor(AActor* NewTarget)
{
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController != nullptr) {
		AIController->GetBlackboardComponent()->SetValueAsObject("TargetActor", NewTarget);
	}
}

void ARLAICharacter::OnHealthChanged(AActor* InstigatorActor, URLAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	// we don't wanna damage yourself
	if (InstigatorActor == this) {
		return;
	}
	SetTargetActor(InstigatorActor); // if AI damage another AI it will set target AI for AI
	
	// we wanna be sure we take any damage
	// I think there are should be 2 function, like Damage & Heal based on Delta
	if (Delta >= 0.0f) {
		return;
	}

	// Is Damaged
	if (Delta < 0.0f)
	{
		if (ActiveHealthBarWidget == nullptr)
		{
			ActiveHealthBarWidget = CreateWidget<URLWorldUserWidget>(GetWorld(), HealthBarWidgetClass);
			if (ActiveHealthBarWidget != nullptr)
			{
				ActiveHealthBarWidget->AttachedActor = this;
				ActiveHealthBarWidget->AddToViewport();
				ActiveHealthBarWidget->CustomInitialize();
			}
		}
		
		float ParameterValue = FMath::Abs(Delta) / AttributeComponent->GetMaxHealth();
		//UE_LOG(LogTemp,Warning,TEXT("%S: ParameterValue = %f"), __FUNCTION__, ParameterValue);
		GetMesh()->SetScalarParameterValueOnMaterials(HitDamageParamName, ParameterValue);

		FTimerHandle TimerHandle_HideHitDamageEffect;
		GetWorldTimerManager().SetTimer(TimerHandle_HideHitDamageEffect, this, &ARLAICharacter::HideHitDamageEffect, HideDamageHitEffectDelay);
	}

	// Is Dead
	if (NewHealth <= 0.0f)
	{
		//ActiveHealthBarWidget->RemoveFromParent(); //maybe it should be here, but im not sure
		
		// Stop BT
		AAIController* AIController = Cast<AAIController>(GetController());
		if (AIController != nullptr) {
			AIController->GetBrainComponent()->StopLogic("Killed");
		}
		
		// Ragdoll
		GetMesh()->SetAllBodiesSimulatePhysics(true);
		GetMesh()->SetCollisionProfileName("Ragdoll");

		// Set lifespan to destroy
		SetLifeSpan(10.f);
	}
}

void ARLAICharacter::HideHitDamageEffect()
{
	GetMesh()->SetScalarParameterValueOnMaterials(HitDamageParamName, 0.0f);
}

float ARLAICharacter::GetDamageAmount()
{
	return DamageAmount;
}

FName ARLAICharacter::GetMuzzleShotSocketName()
{
	return MuzzleShotSocketName;
}



