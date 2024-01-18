// Fill out your copyright notice in the Description page of Project Settings.

#include "RLCharacter.h"

#include "ActionRoguelike/ActorComponents/RLActionComponent.h"
#include "ActionRoguelike/ActorComponents/RLAttributeComponent.h"
#include "ActionRoguelike/ActorComponents/RLInteractionComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"


ARLCharacter::ARLCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->SetupAttachment(RootComponent);
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);

	InteractionComponent = CreateDefaultSubobject<URLInteractionComponent>("InteractionComponent");
	AttributeComponent = CreateDefaultSubobject<URLAttributeComponent>("AttributeComponent");
	ActionComp = CreateDefaultSubobject<URLActionComponent>("ActionComp");
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	
	bUseControllerRotationYaw = false;

	HitDamageParamName = "Damage";
	HideDamageHitEffectDelay = 0.2f;
	
	DamageAmount = 50.f;
}

void ARLCharacter::BeginPlay()
{
	Super::BeginPlay();

}

void ARLCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	AttributeComponent->OnHealthChanged.AddDynamic(this, &ARLCharacter::OnHealthChanged);
}

void ARLCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARLCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Check review video on 'Life EXE' channel
	// https://youtu.be/dUmXk-p43oo?si=zljlT5zBWL-nnKJy

	// MOVEMENT
	PlayerInputComponent->BindAxis("MoveForward", this, &ARLCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ARLCharacter::MoveRight);

	// ROTATION CONTROL
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	// ACTIONS
	// Movement
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ARLCharacter::SprintStart);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ARLCharacter::SprintStop);

	// Attack
	// Maybe later add Secondary Attack (some big shot with splash maybe), rename BlackHole to "Ultimate Attack", Dash to smth 
	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &ARLCharacter::PrimaryAttack);

	// rework into teleport ability to place where is projectile maybe
	PlayerInputComponent->BindAction("DashAttack", IE_Pressed, this, &ARLCharacter::DashAttack);

	// rework to black hole where projectile hit maybe
	PlayerInputComponent->BindAction("BlackHoleAttack", IE_Pressed, this, &ARLCharacter::BlackHoleAttack);

	//Interact
	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &ARLCharacter::PrimaryInteract);
}

void ARLCharacter::MoveForward(float Value)
{
	FRotator ControlRotation = GetControlRotation();
	ControlRotation.Pitch = 0.0f;
	ControlRotation.Roll = 0.0f;
	
	AddMovementInput(ControlRotation.Vector(), Value);
}

void ARLCharacter::MoveRight(float Value)
{
	FRotator ControlRotation = GetControlRotation();
	ControlRotation.Pitch = 0.0f;
	ControlRotation.Roll = 0.0f;

	FVector RightVector = FRotationMatrix(ControlRotation).GetScaledAxis(EAxis::Y);
	
	AddMovementInput(RightVector, Value);
}

void ARLCharacter::SprintStart()
{
	ActionComp->StartActionByName(this, "Sprint");
}

void ARLCharacter::SprintStop()
{
	ActionComp->StopActionByName(this, "Sprint");
}

void ARLCharacter::PrimaryAttack()
{
	ActionComp->StartActionByName(this, "PrimaryAttack");
}

void ARLCharacter::DashAttack()
{
	ActionComp->StartActionByName(this, "DashAttack");
}

void ARLCharacter::BlackHoleAttack()
{
	ActionComp->StartActionByName(this, "BlackHoleAttack");
}

void ARLCharacter::PrimaryInteract()
{
	InteractionComponent->PrimaryInteract();
}

void ARLCharacter::OnHealthChanged(AActor* InstigatorActor, URLAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (InstigatorActor == this) {
		return;
	}
	
	if (Delta >= 0.0f) {
		return;
	}

	// Is Damaged
	if (Delta < 0.0f)
	{
		float ParameterValue = FMath::Abs(Delta) / AttributeComponent->GetMaxHealth();
		//UE_LOG(LogTemp,Warning,TEXT("%S: ParameterValue = %f"), __FUNCTION__, ParameterValue);
		GetMesh()->SetScalarParameterValueOnMaterials(HitDamageParamName, ParameterValue);

		FTimerHandle TimerHandle_HideHitDamageEffect;
		GetWorldTimerManager().SetTimer(TimerHandle_HideHitDamageEffect, this, &ARLCharacter::HideHitDamageEffect, HideDamageHitEffectDelay);
	}
	
	// Is Dead
	if (NewHealth <= 0.0f)
	{
		APlayerController* PlayerController = Cast<APlayerController>(GetController());
		DisableInput(PlayerController);

		// don't forget to disable capsule
	}
}

void ARLCharacter::HideHitDamageEffect()
{
	GetMesh()->SetScalarParameterValueOnMaterials(HitDamageParamName, 0.0f);
	//UE_LOG(LogTemp,Warning,TEXT("%S:"), __FUNCTION__);
}

void ARLCharacter::HealSelf(float Amount)
{
	AttributeComponent->ApplyHealthChange(this, Amount);
}

FVector ARLCharacter::GetPawnViewLocation() const
{
	//It's ok for first person, but not third person with aim crosshair in center of screen
	//return Super::GetPawnViewLocation();

	// used in GetActorEyesViewPoint
	return CameraComponent->GetComponentLocation();
}

float ARLCharacter::GetDamageAmount()
{
	return DamageAmount;
}





