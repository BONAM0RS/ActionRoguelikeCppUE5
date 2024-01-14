// Fill out your copyright notice in the Description page of Project Settings.


#include "RLWorldUserWidget.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/SizeBox.h"
#include "Kismet/GameplayStatics.h"


URLWorldUserWidget::URLWorldUserWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer),
	  AttachedActor(nullptr),
	  ParentSizeBox(nullptr)
{
}

void URLWorldUserWidget::CustomInitialize_Implementation()
{
}

void URLWorldUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!IsValid(AttachedActor))
	{
		RemoveFromParent();
		UE_LOG(LogTemp,Warning,TEXT("AttachedActor is no longer valid, removing Health Widget"));
		return;
	}

	FVector2D ScreenPosition;
	if (UGameplayStatics::ProjectWorldToScreen(GetOwningPlayer(), AttachedActor->GetActorLocation() + WorldOffset, ScreenPosition))
	{
		float ViewportScale = UWidgetLayoutLibrary::GetViewportScale(this);
		ScreenPosition = ScreenPosition / ViewportScale;
		if (ParentSizeBox != nullptr) {
			ParentSizeBox->SetRenderTranslation(ScreenPosition);
		}
	}
}
