// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RLInteractionComponent.generated.h"

class URLWorldUserWidget;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API URLInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	URLInteractionComponent();

protected:
	virtual void BeginPlay() override;
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void FindBestInteractable();

public:
	void PrimaryInteract();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Config")
	float TraceDistance;

	UPROPERTY(EditDefaultsOnly, Category = "Config")
	float TraceRadius;

	UPROPERTY(EditDefaultsOnly, Category = "Config")
	TEnumAsByte<ECollisionChannel> TraceCollisionChannel;
	
	UPROPERTY()
	AActor* FocusedActor;

	UPROPERTY(EditDefaultsOnly, Category = "Config")
	TSubclassOf<URLWorldUserWidget> DefaultWidgetClass;

	UPROPERTY()
	URLWorldUserWidget* DefaultWidgetInstance;
};
