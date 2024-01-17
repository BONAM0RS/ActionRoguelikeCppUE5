// Fill out your copyright notice in the Description page of Project Settings.


#include "RLAction_ProjectileAttack.h"

#include "ActionRoguelike/RLMageProjectile.h"
#include "ActionRoguelike/Core/RLCharacter.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

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
		Character->PlayAnimMontage(AttackAnim);

		UGameplayStatics::SpawnEmitterAttached(CastingEffect, Character->GetMesh(), MuzzleSocketName,
		FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget);

		FTimerHandle TimerHandle_AttackDelay;
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "AttackDelay_Elapsed", Character);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_AttackDelay, Delegate, AttackAnimDelay, false);
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
