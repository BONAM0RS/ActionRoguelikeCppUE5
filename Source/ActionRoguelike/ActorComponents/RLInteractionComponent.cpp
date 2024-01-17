// Fill out your copyright notice in the Description page of Project Settings.

#include "RLInteractionComponent.h"

#include "ActionRoguelike/Interfaces/RLGameplayInterface.h"


static TAutoConsoleVariable<bool> CVarDebugDrawInteraction(TEXT("su.InteractionDebugDraw"), false,
	TEXT("Enable Debug Lines for Interact Component"), ECVF_Cheat);


URLInteractionComponent::URLInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void URLInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

}

void URLInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void URLInteractionComponent::PrimaryInteract()
{
	AActor* Owner = GetOwner();
	FVector ViewPointLocation;
	FRotator ViewPointRotation;
	Owner->GetActorEyesViewPoint(ViewPointLocation, ViewPointRotation);
	
	FVector StartLocation = ViewPointLocation;
	FVector EndLocation = ViewPointLocation + (ViewPointRotation.Vector() * 1000.f);

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	FCollisionShape CollisionShape;
	float Radius = 30.0f;
	CollisionShape.SetSphere(Radius);
	
	TArray<FHitResult> HitResults;
	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(HitResults, StartLocation, EndLocation, FQuat::Identity, ObjectQueryParams, CollisionShape);

	bool bDebugDraw = CVarDebugDrawInteraction.GetValueOnGameThread();
	FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;
	
	for (FHitResult HitResult: HitResults)
	{
		if (bDebugDraw) {
			DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, Radius, 32, LineColor, false, 2.0f);
		}
		
		AActor* HitActor = HitResult.GetActor();
		if (HitActor != nullptr)
		{
			if (HitActor->Implements<URLGameplayInterface>())
			{
				APawn* Pawn = Cast<APawn>(Owner);
				IRLGameplayInterface::Execute_Interact(HitActor, Pawn);
				break;
			}
		}
	}

	if (bDebugDraw) {
		DrawDebugLine(GetWorld(), StartLocation, EndLocation, LineColor, false, 2.0f, 0, 2.0f);
	}
}

