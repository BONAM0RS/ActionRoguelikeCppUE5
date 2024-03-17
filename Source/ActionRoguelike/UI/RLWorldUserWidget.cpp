/*
 * Third-person Action Roguelike game prototype made in Unreal Engine 5.3 as part of the Professional Game Development
 * in C++ and Unreal Engine course by Tom Looman.
 * Licensed for use only with UE based products.
 */


#include "RLWorldUserWidget.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/SizeBox.h"
#include "Kismet/GameplayStatics.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(RLWorldUserWidget)


URLWorldUserWidget::URLWorldUserWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer),
	  AttachedActor(nullptr),
	  ParentSizeBox(nullptr),
	  ShowDistance(6000.f),
	  CurrentDistanceToPlayer(0.0f),
	  FullRenderScaleDistance(3000.0f),
	  MinRenderScale(0.5f),
	  MaxRenderScale(1.0f),
	  RenderScale(1.0f)
{
}

void URLWorldUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	SetVisibility(ESlateVisibility::Collapsed);

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &URLWorldUserWidget::ConvertWorldLocationToScreenLocation, 0.01f, true);
}

void URLWorldUserWidget::NativeDestruct()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	
	Super::NativeDestruct();
}

void URLWorldUserWidget::CustomInitialize_Implementation()
{
}

void URLWorldUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void URLWorldUserWidget::ConvertWorldLocationToScreenLocation_Implementation()
{
	if (!IsValid(AttachedActor))
	{
		RemoveFromParent();
		UE_LOG(LogTemp,Warning,TEXT("AttachedActor is no longer valid, removing Health Widget"));
		return;
	}

	FVector2D ScreenPosition;
	bool bIsOnScreen = UGameplayStatics::ProjectWorldToScreen(GetOwningPlayer(), AttachedActor->GetActorLocation() + WorldOffset, ScreenPosition);
	if (bIsOnScreen)
	{
		CalculateAndApplyRenderScalePlusAlignment();
		
		if (CurrentDistanceToPlayer < ShowDistance)
		{
			float ViewportScale = UWidgetLayoutLibrary::GetViewportScale(this);
			FVector2D ViewportPosition = ScreenPosition / ViewportScale;
			SetPositionInViewport(ViewportPosition, false);
			SetVisibility(ESlateVisibility::HitTestInvisible);
		}
		else
		{
			SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	else
	{
		SetVisibility(ESlateVisibility::Collapsed);
	}
}

void URLWorldUserWidget::CalculateAndApplyRenderScalePlusAlignment()
{
	FVector PlayerPosition = GetOwningPlayerPawn()->GetActorLocation();
	FVector AttachedActorPosition = AttachedActor->GetActorLocation();
	CurrentDistanceToPlayer = FVector::Distance(PlayerPosition, AttachedActorPosition);
	float Ratio = FullRenderScaleDistance / CurrentDistanceToPlayer;
	RenderScale = FMath::Clamp(Ratio, MinRenderScale, MaxRenderScale);
	FVector2D RenderScale2D = FVector2D(RenderScale,RenderScale);
	SetRenderScale(RenderScale2D);

	CalculateAndApplyAlignment();
}

void URLWorldUserWidget::CalculateAndApplyAlignment()
{
	float AlignmentX = FMath::Max(MaxRenderScale - (RenderScale - MinRenderScale) / (MaxRenderScale - MinRenderScale), MinRenderScale);
	float AlignmentY = MinRenderScale + AlignmentX;
	
	FVector2D Alignment2D = FVector2D(AlignmentX, AlignmentY);
	UWidgetLayoutLibrary::SlotAsCanvasSlot(ParentSizeBox)->SetAlignment(Alignment2D);
}
