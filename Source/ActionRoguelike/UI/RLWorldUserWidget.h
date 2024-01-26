// Fill out your copyright notice in the Description page of Project Settings.

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

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void CustomInitialize();

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	UPROPERTY(BlueprintReadOnly, meta = (ExposeOnSpawn = "true"), Category = "UI")
	AActor* AttachedActor;

protected:
	UPROPERTY(meta = (BindWidget))
	USizeBox* ParentSizeBox;

	UPROPERTY(EditAnywhere, Category = "Config")
	FVector WorldOffset;
};
