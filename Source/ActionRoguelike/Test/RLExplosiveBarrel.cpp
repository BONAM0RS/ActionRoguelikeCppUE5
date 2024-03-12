/*
 * Third-person Action Roguelike game prototype made in Unreal Engine 5.3 as part of the Professional Game Development
 * in C++ and Unreal Engine course by Tom Looman.
 * Licensed for use only with UE based products.
 */

#include "RLExplosiveBarrel.h"

#include "PhysicsEngine/RadialForceComponent.h"


ARLExplosiveBarrel::ARLExplosiveBarrel()
{
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	StaticMeshComponent->SetSimulatePhysics(true);
	StaticMeshComponent->SetCollisionProfileName("PhysicsActor");
	RootComponent = StaticMeshComponent;
	
	RadialForceComponent = CreateDefaultSubobject<URadialForceComponent>("RadialForceComponent");
	RadialForceComponent->SetAutoActivate(false);
	RadialForceComponent->Radius = 500.0f;
	
	// Alternative: 200 000.0f if bImpulseVelChange = false;
	RadialForceComponent->ImpulseStrength = 1000.0f;
	
	// Optional, ignores 'Mass' of other objects (if false, the impulse strength should be much higher to push most objects depending on Mass)
	RadialForceComponent->bImpulseVelChange = true;
	
	// Optional, default constructor already adds 4 object types to affect, excluding WorldDynamic
	RadialForceComponent->AddCollisionChannelToAffect(ECC_WorldDynamic);
	
	RadialForceComponent->SetupAttachment(StaticMeshComponent);
}

void ARLExplosiveBarrel::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	StaticMeshComponent->OnComponentHit.AddDynamic(this, &ARLExplosiveBarrel::OnMeshComponentHit);
}

void ARLExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();
}

void ARLExplosiveBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARLExplosiveBarrel::OnMeshComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	RadialForceComponent->FireImpulse();
	//UE_LOG(LogTemp, Warning, TEXT("%S: OtherActor = %s, game time = %f"), __FUNCTION__, *GetNameSafe(OtherActor), GetWorld()->TimeSeconds);

	// FString Message = FString::Printf(TEXT("Hit at location: %s"), *Hit.ImpactPoint.ToString());
	// DrawDebugString(GetWorld(), Hit.ImpactPoint, Message, nullptr, FColor::Blue, 5.0f, true);
}

