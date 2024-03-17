/*
 * Third-person Action Roguelike game prototype made in Unreal Engine 5.3 as part of the Professional Game Development
 * in C++ and Unreal Engine course by Tom Looman.
 * Licensed for use only with UE based products.
 */

#include "RLCharacter.h"

#include "ActionRoguelike/Components/RLActionComponent.h"
#include "ActionRoguelike/Components/RLAttributeComponent.h"
#include "ActionRoguelike/Components/RLInteractionComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(RLCharacter)


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

	// MOVEMENT
	PlayerInputComponent->BindAxis("MoveForward", this, &ARLCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ARLCharacter::MoveRight);
	
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);

	// ROTATION
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	// INTERACT
	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &ARLCharacter::PrimaryInteract);
	
	// ACTIONS
	const TMap<FName, FName> PressedActions = {
			{ "SprintAction", "Sprint" },
			{ "PrimaryAction", "MageAttack" },
			{ "AdditionalAction", "DashAttack" },
			{ "UltimateAction", "BlackHoleAttack" }
		};

	for (const auto& [InputActionName, ActionName] : PressedActions)
	{
		FInputActionBinding InputActionBinding(InputActionName, IE_Pressed);
		InputActionBinding.ActionDelegate.GetDelegateForManualSet().BindLambda(
			[&, ActionName]()
			{
				ActionComp->StartActionByName(this, ActionName);
			});
		PlayerInputComponent->AddActionBinding(InputActionBinding);
	}

	// SecondaryAction implemented in blueprints (Parry)
	
	PlayerInputComponent->BindAction("SprintAction", IE_Released, this, &ARLCharacter::SprintStop);
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

void ARLCharacter::SprintStop()
{
	ActionComp->StopActionByName(this, "Sprint");
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
		GetMesh()->SetScalarParameterValueOnMaterials(HitDamageParamName, ParameterValue);
		//UE_LOG(LogTemp,Warning,TEXT("%S: ParameterValue = %f"), __FUNCTION__, ParameterValue);

		FTimerHandle TimerHandle_HideHitDamageEffect;
		GetWorldTimerManager().SetTimer(TimerHandle_HideHitDamageEffect, this, &ARLCharacter::HideHitDamageEffect, HideDamageHitEffectDelay);

		// Rage added equal to damage received (Abs to turn into positive rage number)
		float RageDelta = FMath::Abs(Delta);
		AttributeComponent->ApplyRageChange(InstigatorActor, RageDelta);
	}
	
	// Is Dead
	if (NewHealth <= 0.0f)
	{
		APlayerController* PlayerController = Cast<APlayerController>(GetController());
		DisableInput(PlayerController);
		
		// Disable capsule on hit event in case player died in jump for example, so he will not fall through the textures
		GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &ARLCharacter::OnDiedCapsuleHit);

		// Destroy 
		SetLifeSpan(10.0f);
	}
}

void ARLCharacter::OnDiedCapsuleHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	GetCapsuleComponent()->OnComponentHit.RemoveDynamic(this, &ARLCharacter::OnDiedCapsuleHit);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
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
	// Offset to not hit the wall if camera collide with it
	FVector Offset = CameraComponent->GetForwardVector() * 100.f;
	return CameraComponent->GetComponentLocation() + Offset;
}

UCameraComponent* ARLCharacter::GetCameraComponent() const
{
	return CameraComponent;
}

float ARLCharacter::GetDamageAmount()
{
	return DamageAmount;
}





