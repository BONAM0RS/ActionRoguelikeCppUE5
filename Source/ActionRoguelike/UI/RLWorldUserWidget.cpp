/*
 * Third-person Action Roguelike game prototype made in Unreal Engine 5.3 as part of the Professional Game Development
 * in C++ and Unreal Engine course by Tom Looman.
 * Licensed for use only with UE based products.
 */


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
	bool bIsOnScreen = UGameplayStatics::ProjectWorldToScreen(GetOwningPlayer(), AttachedActor->GetActorLocation() + WorldOffset, ScreenPosition);
	if (bIsOnScreen)
	{
		float ViewportScale = UWidgetLayoutLibrary::GetViewportScale(this);
		ScreenPosition = ScreenPosition / ViewportScale;
		if (ParentSizeBox != nullptr) {
			ParentSizeBox->SetRenderTranslation(ScreenPosition);
		}
	}

	if (ParentSizeBox)
	{
		ParentSizeBox->SetVisibility(bIsOnScreen ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
	}
}
