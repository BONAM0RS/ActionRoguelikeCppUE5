/*
 * Third-person Action Roguelike game prototype made in Unreal Engine 5.3 as part of the Professional Game Development
 * in C++ and Unreal Engine course by Tom Looman.
 * Licensed for use only with UE based products.
 */

#include "RLAction_ProjectileAttack.h"

#include "ActionRoguelike/Core/RLCharacter.h"
#include "ActionRoguelike/Projectiles/RLMageProjectile.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"


#include UE_INLINE_GENERATED_CPP_BY_NAME(RLAction_ProjectileAttack)


URLAction_ProjectileAttack::URLAction_ProjectileAttack()
	: AttackAnim(nullptr),
	  CastingEffect(nullptr)
{
	MuzzleSocketName = "Muzzle_01";
	AttackAnimDelay = 0.2f;

	TraceLength = 5000.f;
	TraceSphereRadius = 20.f;
}

void URLAction_ProjectileAttack::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);
	
	ACharacter* Character = Cast<ACharacter>(Instigator);
	if (Character != nullptr)
	{
		// Rotate character in attack direction
		ARLCharacter* PlayerCharacter = Cast<ARLCharacter>(Character);
		if (PlayerCharacter)
		{
			FVector CameraForwardVector = PlayerCharacter->GetCameraComponent()->GetForwardVector();
			FRotator CameraRotation = CameraForwardVector.Rotation();
			CameraRotation.Pitch = 0.0f;
			CameraRotation.Roll = 0.0f;
			
			PlayerCharacter->SetActorRotation(CameraRotation);
		}
		
		Character->PlayAnimMontage(AttackAnim);

		UGameplayStatics::SpawnEmitterAttached(CastingEffect, Character->GetMesh(), MuzzleSocketName,
		FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget);

		// Is Server?
		if (Character->HasAuthority())
		{
			FTimerHandle TimerHandle_AttackDelay;
			FTimerDelegate Delegate;
			Delegate.BindUFunction(this, "AttackDelay_Elapsed", Character);
			SetWorldTimer(TimerHandle_AttackDelay, Delegate, AttackAnimDelay, false);
		}
	}
}

void URLAction_ProjectileAttack::AttackDelay_Elapsed(ACharacter* InstigatorCharacter)
{
	if (ensureAlways(ProjectileClass))
	{
		FVector ProjectileTargetPoint = CalculateProjectileTargetPoint(InstigatorCharacter);
		
		FVector PrimaryHandLocation = InstigatorCharacter->GetMesh()->GetSocketLocation(MuzzleSocketName);
		FRotator RotatorToTargetPoint = FRotationMatrix::MakeFromX(ProjectileTargetPoint - PrimaryHandLocation).Rotator();
		FTransform SpawnTransform = FTransform(RotatorToTargetPoint, PrimaryHandLocation);
		
		AActor* NewProjectile = GetWorld()->SpawnActorDeferred<AActor>(ProjectileClass,
			SpawnTransform, InstigatorCharacter, InstigatorCharacter, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		ARLMageProjectile* MageProjectile = Cast<ARLMageProjectile>(NewProjectile);
		if (MageProjectile != nullptr)
		{
			ARLCharacter* PlayerCharacter = Cast<ARLCharacter>(InstigatorCharacter);
			if (PlayerCharacter != nullptr) {
				MageProjectile->SetDamageAmount(PlayerCharacter->GetDamageAmount());
			}
		}
		UGameplayStatics::FinishSpawningActor(NewProjectile, SpawnTransform);
	}

	StopAction(InstigatorCharacter);
}

FVector URLAction_ProjectileAttack::CalculateProjectileTargetPoint(ACharacter* InstigatorCharacter)
{
	FVector TraceStart = InstigatorCharacter->GetPawnViewLocation();
	FVector TraceEnd = TraceStart + (InstigatorCharacter->GetControlRotation().Vector() * TraceLength);
	
	FCollisionObjectQueryParams CollisionObjectQueryParams;
	CollisionObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	CollisionObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
	CollisionObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);

	FCollisionShape CollisionShape;
	CollisionShape.SetSphere(TraceSphereRadius);

	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(InstigatorCharacter);
		
	FHitResult HitResult;
	bool bHitSuccess = GetWorld()->SweepSingleByObjectType(HitResult, TraceStart, TraceEnd, FQuat::Identity,
												CollisionObjectQueryParams, CollisionShape, CollisionQueryParams);
	if (bHitSuccess) {
		return HitResult.ImpactPoint;
	}
	
	return TraceEnd;
}
