// Fill out your copyright notice in the Description page of Project Settings.

#include "RLBTTaskNode_RangeAttack.h"

#include "AIController.h"
#include "RLAICharacter.h"
#include "ActionRoguelike/ActorComponents/RLAttributeComponent.h"
#include "ActionRoguelike/Projectiles/RLMageProjectile.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"


URLBTTaskNode_RangeAttack::URLBTTaskNode_RangeAttack()
{
	MaxBulletSpread = 2.0f;
}

EBTNodeResult::Type URLBTTaskNode_RangeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (ensure(AIController))
	{
		ARLAICharacter* AICharacter = Cast<ARLAICharacter>(AIController->GetPawn());
		if (AICharacter == nullptr) {
			return EBTNodeResult::Failed;
		}
		
		AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetActor"));
		if (TargetActor == nullptr) {
			return EBTNodeResult::Failed;
		}

		if (!URLAttributeComponent::IsActorAlive(TargetActor)) {
			return EBTNodeResult::Failed;
		}
		
		FVector MuzzleLocation = AICharacter->GetMesh()->GetSocketLocation(AICharacter->GetMuzzleShotSocketName());
		FVector Direction = TargetActor->GetActorLocation() - MuzzleLocation;
		FRotator MuzzleRotation = Direction.Rotation();

		MuzzleRotation.Pitch += FMath::RandRange(0.0f, MaxBulletSpread);
		MuzzleRotation.Yaw += FMath::RandRange(-MaxBulletSpread, MaxBulletSpread);

		//FActorSpawnParameters ActorSpawnParams;
		//ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		// I need to set projectile damage from AiCharacter here (READY)
		//AActor* NewProjectile = GetWorld()->SpawnActor<AActor>(ProjectileClass, MuzzleLocation, MuzzleRotation, ActorSpawnParams); // old way
		FTransform SpawnTransform = FTransform(MuzzleRotation, MuzzleLocation);
		AActor* NewProjectile = GetWorld()->SpawnActorDeferred<AActor>(ProjectileClass,
			SpawnTransform, AICharacter, AICharacter, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		ARLMageProjectile* MageProjectile = Cast<ARLMageProjectile>(NewProjectile);
		if (MageProjectile != nullptr) {
			MageProjectile->SetDamageAmount(AICharacter->GetDamageAmount());
		}
		UGameplayStatics::FinishSpawningActor(NewProjectile, SpawnTransform);

		return NewProjectile ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
	}
	
	return EBTNodeResult::Failed;
}
