// Fill out your copyright notice in the Description page of Project Settings.

#include "RLCharacter.h"

#include "ActionRoguelike/RLProjectileBase.h"
#include "ActionRoguelike/ActorComponents/RLAttributeComponent.h"
#include "ActionRoguelike/ActorComponents/RLInteractionComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"


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
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	
	bUseControllerRotationYaw = false;

	AttackAnimDelay = 0.2f;
	
	HideDamageHitEffectDelay = 0.2f;
	
	TraceLength = 5000.f;
	TraceSphereRadius = 20.f;

	PrimaryHandSocketName = "Muzzle_01";
	HitDamageParamName = "Damage";
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

	//Movement
	PlayerInputComponent->BindAxis("MoveForward", this, &ARLCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ARLCharacter::MoveRight);

	//Rotation control
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	//Actions
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	
	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &ARLCharacter::PrimaryAttack);
	PlayerInputComponent->BindAction("DashAttack", IE_Pressed, this, &ARLCharacter::DashAttack);
	PlayerInputComponent->BindAction("BlackHoleAttack", IE_Pressed, this, &ARLCharacter::BlackHoleAttack);

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

void ARLCharacter::PrimaryAttack()
{
	StartAttackEffect();
	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ARLCharacter::PrimaryAttack_TimeElapsed, AttackAnimDelay);
}

void ARLCharacter::PrimaryAttack_TimeElapsed()
{
	SpawnProjectile(PrimaryProjectileClass);
}

void ARLCharacter::DashAttack()
{
	StartAttackEffect();
	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ARLCharacter::DashAttack_TimeElapsed, AttackAnimDelay);
}

void ARLCharacter::DashAttack_TimeElapsed()
{
	SpawnProjectile(DashProjectileClass);
}

void ARLCharacter::BlackHoleAttack()
{
	StartAttackEffect();
	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ARLCharacter::BlackHoleAttack_TimeElapsed, AttackAnimDelay);
}

void ARLCharacter::BlackHoleAttack_TimeElapsed()
{
	SpawnProjectile(BlackHoleProjectileClass);
}

void ARLCharacter::StartAttackEffect()
{
	PlayAnimMontage(AttackAnim);

	UGameplayStatics::SpawnEmitterAttached(CastingEffect, GetMesh(), PrimaryHandSocketName,
		FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget);
}

void ARLCharacter::SpawnProjectile(TSubclassOf<AActor> ProjectileClassToSpawn)
{
	if (ensureAlways(ProjectileClassToSpawn))
	{
		FVector ProjectileTargetPoint = CalculateProjectileTargetPoint();
		
		FVector PrimaryHandLocation = GetMesh()->GetSocketLocation(PrimaryHandSocketName);
		FRotator RotatorToTargetPoint = FRotationMatrix::MakeFromX(ProjectileTargetPoint - PrimaryHandLocation).Rotator();
		FTransform SpawnTransform = FTransform(RotatorToTargetPoint, PrimaryHandLocation);
		
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = this;
		
		GetWorld()->SpawnActor<AActor>(ProjectileClassToSpawn, SpawnTransform, SpawnParams);
	}
}

FVector ARLCharacter::CalculateProjectileTargetPoint()
{
	FVector TraceStart = CameraComponent->GetComponentLocation();
	FVector TraceEnd = TraceStart + (GetControlRotation().Vector() * TraceLength);

	FCollisionObjectQueryParams CollisionObjectQueryParams;
	CollisionObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	CollisionObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
	CollisionObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);

	FCollisionShape CollisionShape;
	CollisionShape.SetSphere(TraceSphereRadius);

	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(this);
		
	FHitResult HitResult;
	bool bHitSuccess = GetWorld()->SweepSingleByObjectType(HitResult, TraceStart, TraceEnd, FQuat::Identity,
												CollisionObjectQueryParams, CollisionShape, CollisionQueryParams);
	if (bHitSuccess) {
		return HitResult.ImpactPoint;
	}
	
	return TraceEnd;
}

void ARLCharacter::PrimaryInteract()
{
	InteractionComponent->PrimaryInteract();
}

void ARLCharacter::OnHealthChanged(AActor* InstigatorActor, URLAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (Delta >= 0.0f) {
		return;
	}

	// Is Damaged
	if (Delta < 0.0f)
	{
		float ParameterValue = FMath::Abs(Delta) / AttributeComponent->GetMaxHealth();
		UE_LOG(LogTemp,Warning,TEXT("%S: ParameterValue = %f"), __FUNCTION__, ParameterValue);
		GetMesh()->SetScalarParameterValueOnMaterials(HitDamageParamName, ParameterValue);

		FTimerHandle TimerHandle_HideHitDamageEffect;
		GetWorldTimerManager().SetTimer(TimerHandle_HideHitDamageEffect, this, &ARLCharacter::HideHitDamageEffect, HideDamageHitEffectDelay);
	}
	
	// Is Dead
	if (NewHealth <= 0.0f)
	{
		APlayerController* PlayerController = Cast<APlayerController>(GetController());
		DisableInput(PlayerController);
	}
}

void ARLCharacter::HideHitDamageEffect()
{
	GetMesh()->SetScalarParameterValueOnMaterials(HitDamageParamName, 0.0f);
	UE_LOG(LogTemp,Warning,TEXT("%S:"), __FUNCTION__);
}




