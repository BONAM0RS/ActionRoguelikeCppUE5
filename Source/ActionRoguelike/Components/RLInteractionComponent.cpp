/*
 * Third-person Action Roguelike game prototype made in Unreal Engine 5.3 as part of the Professional Game Development
 * in C++ and Unreal Engine course by Tom Looman.
 * Licensed for use only with UE based products.
 */

#include "RLInteractionComponent.h"

#include "ActionRoguelike/Interfaces/RLGameplayInterface.h"
#include "ActionRoguelike/UI/RLWorldUserWidget.h"
#include "Blueprint/UserWidget.h"


static TAutoConsoleVariable<bool> CVarDebugDrawInteraction(TEXT("su.InteractionDebugDraw"), false,
                                                           TEXT("Enable Debug Lines for Interact Component"), ECVF_Cheat);


URLInteractionComponent::URLInteractionComponent()
	: TraceDistance(500.0f),
	  TraceRadius(30.0f),
	  TraceCollisionChannel(ECC_WorldDynamic),
	  FocusedActor(nullptr),
	  DefaultWidgetInstance(nullptr)
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

	APawn* PawnOwner = Cast<APawn>(GetOwner());
	if (PawnOwner->IsLocallyControlled())
	{
		// Better replace with timer instead call it on tick
		FindBestInteractable();
	}
}

void URLInteractionComponent::FindBestInteractable()
{
	AActor* Owner = GetOwner();
	FVector ViewPointLocation;
	FRotator ViewPointRotation;
	Owner->GetActorEyesViewPoint(ViewPointLocation, ViewPointRotation);
	
	FVector StartLocation = ViewPointLocation;
	FVector EndLocation = ViewPointLocation + (ViewPointRotation.Vector() * TraceDistance);

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(TraceCollisionChannel);

	FCollisionShape CollisionShape;
	CollisionShape.SetSphere(TraceRadius);
	
	TArray<FHitResult> HitResults;
	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(HitResults, StartLocation, EndLocation, FQuat::Identity, ObjectQueryParams, CollisionShape);

	bool bDebugDraw = CVarDebugDrawInteraction.GetValueOnGameThread();
	FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;

	// Clear ref before trying to fill
	FocusedActor = nullptr;
	
	for (FHitResult HitResult: HitResults)
	{
		if (bDebugDraw) {
			DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, TraceRadius, 32, LineColor, false, 0.0f);
		}
		
		AActor* HitActor = HitResult.GetActor();
		if (HitActor != nullptr)
		{
			if (HitActor->Implements<URLGameplayInterface>())
			{
				FocusedActor = HitActor;
				break;
			}
		}
	}

	if (bDebugDraw) {
		DrawDebugLine(GetWorld(), StartLocation, EndLocation, LineColor, false, 0.0f, 0, 2.0f);
	}
	
	if (FocusedActor != nullptr)
	{
		if (DefaultWidgetInstance == nullptr && ensure(DefaultWidgetClass))
		{
			DefaultWidgetInstance = CreateWidget<URLWorldUserWidget>(GetWorld(), DefaultWidgetClass);
		}

		if (DefaultWidgetInstance != nullptr)
		{
			DefaultWidgetInstance->AttachedActor = FocusedActor;
			if (!DefaultWidgetInstance->IsInViewport())
			{
				DefaultWidgetInstance->AddToViewport();
			}
		}
	}
	else
	{
		if (DefaultWidgetInstance != nullptr)
		{
			DefaultWidgetInstance->RemoveFromParent();
		}
	}
}

void URLInteractionComponent::PrimaryInteract()
{
	ServerInteract(FocusedActor);
}

void URLInteractionComponent::ServerInteract_Implementation(AActor* InFocusActor)
{
	if (InFocusActor == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "No Focused Actor to Interact");
		return;
	}
	
	APawn* Pawn = Cast<APawn>(GetOwner());
	IRLGameplayInterface::Execute_Interact(InFocusActor, Pawn);
}
