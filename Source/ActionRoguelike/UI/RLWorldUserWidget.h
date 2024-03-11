/*
 * Third-person Action Roguelike game prototype made in Unreal Engine 5.3 as part of the Professional Game Development
 * in C++ and Unreal Engine course by Tom Looman.
 * Licensed for use only with UE based products.
 */

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RLWorldUserWidget.generated.h"


class USizeBox;

UCLASS()
class ACTIONROGUELIKE_API URLWorldUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	URLWorldUserWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void CustomInitialize();

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ConvertWorldLocationToScreenLocation();

	void CalculateAndApplyRenderScalePlusAlignment();
	void CalculateAndApplyAlignment();

public:
	UPROPERTY(BlueprintReadOnly, meta = (ExposeOnSpawn = "true"), Category = "UI")
	AActor* AttachedActor;
	
	FTimerHandle TimerHandle;

protected:
	UPROPERTY(meta = (BindWidget))
	USizeBox* ParentSizeBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	FVector WorldOffset;

	UPROPERTY(EditAnywhere, Category = "Config")
	float ShowDistance;
	
	float CurrentDistanceToPlayer;

	UPROPERTY(EditAnywhere, Category = "Config")
	float FullRenderScaleDistance;

	UPROPERTY(EditAnywhere, Category = "Config")
	float MinRenderScale;
	
	float MaxRenderScale;

	float RenderScale;
};
