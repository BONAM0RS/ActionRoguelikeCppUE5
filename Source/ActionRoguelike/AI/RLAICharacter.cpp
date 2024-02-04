// Fill out your copyright notice in the Description page of Project Settings.

#include "RLAICharacter.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "ActionRoguelike/ActorComponents/RLActionComponent.h"
#include "ActionRoguelike/ActorComponents/RLAttributeComponent.h"
#include "ActionRoguelike/UI/RLWorldUserWidget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/PawnSensingComponent.h"


ARLAICharacter::ARLAICharacter()
{
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");
	AttributeComponent = CreateDefaultSubobject<URLAttributeComponent>("AttributeComp");
	ActionComp = CreateDefaultSubobject<URLActionComponent>("ActionComp");

	// Ensures we receive a controlled when spawned in the level by our gamemode
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// Enabled on mesh to react to incoming projectiles
	GetMesh()->SetGenerateOverlapEvents(true);
	
	// Temp solution
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);

	MuzzleShotSocketName = "Muzzle_Front";
	HitDamageParamName = "Damage";

	HideDamageHitEffectDelay = 0.2f;

	DamageAmount = 5.f;

	ActiveHealthBarWidget = nullptr;

	TargetActorKey = "TargetActor";
}

void ARLAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	PawnSensingComp->OnSeePawn.AddDynamic(this, &ARLAICharacter::OnPawnSeen);
	AttributeComponent->OnHealthChanged.AddDynamic(this, &ARLAICharacter::OnHealthChanged);
}

void ARLAICharacter::OnPawnSeen(APawn* Pawn)
{
	// Ignore if target already set
	if (GetTargetActor() != Pawn)
	{
		SetTargetActor(Pawn);

		MulticastPawnSeen();
	}
	
	//DrawDebugString(GetWorld(), GetActorLocation(), "PLAYER SPOTTED", nullptr, FColor::White, 4.0f, true);
}

void ARLAICharacter::MulticastPawnSeen_Implementation()
{
	URLWorldUserWidget* NewWidget = CreateWidget<URLWorldUserWidget>(GetWorld(), SpottedWidgetClass);
	if (NewWidget)
	{
		NewWidget->AttachedActor = this;
		// Index of 10 (or anything higher than default of 0) places this on top of any other widget.
		// May end up behind the minion health bar otherwise.
		NewWidget->AddToViewport(10);
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
	//TODO: I think there are should be 2 function, like Damage & Heal based on Delta
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

		// Disable capsule collision and movement
		GetCharacterMovement()->DisableMovement();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		
		// Set lifespan to destroy
		SetLifeSpan(10.f);
	}
}

void ARLAICharacter::SetTargetActor(AActor* NewTarget)
{
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController != nullptr) {
		AIController->GetBlackboardComponent()->SetValueAsObject(TargetActorKey, NewTarget);
	}
}

void ARLAICharacter::HideHitDamageEffect()
{
	GetMesh()->SetScalarParameterValueOnMaterials(HitDamageParamName, 0.0f);
}

AActor* ARLAICharacter::GetTargetActor() const
{
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController != nullptr)
	{
		return Cast<AActor>(AIController->GetBlackboardComponent()->GetValueAsObject(TargetActorKey));
	}

	return nullptr;
}

float ARLAICharacter::GetDamageAmount()
{
	return DamageAmount;
}

FName ARLAICharacter::GetMuzzleShotSocketName()
{
	return MuzzleShotSocketName;
}



