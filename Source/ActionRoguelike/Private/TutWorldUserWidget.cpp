// Fill out your copyright notice in the Description page of Project Settings.


#include "TutWorldUserWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/SizeBox.h"
#include "Kismet/GameplayStatics.h"

void UTutWorldUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);


	// protect if actor nullptr or ispendingkill
	if (!IsValid(AttachedActor))
	{
		RemoveFromParent(); 

		UE_LOG(LogTemp, Warning, TEXT("AttachedActor no longer valid, removing Health Widget."));
		return;
	}

	FVector2D ScreenPosition;
	bool bIsOnScreen = UGameplayStatics::ProjectWorldToScreen(GetOwningPlayer(), AttachedActor->GetActorLocation() + WorldOffset, ScreenPosition);
	
	// will be true if a position onscreen is found, false if offscreen
	if (UGameplayStatics::ProjectWorldToScreen(GetOwningPlayer(), AttachedActor->GetActorLocation() + WorldOffset, ScreenPosition))
	{
		float Scale = UWidgetLayoutLibrary::GetViewportScale(this);

		ScreenPosition /= Scale; //Get the correct position based on the current resolution, via the dpi scale

		if (ParentSizeBox)
		{
			ParentSizeBox->SetRenderTranslation(ScreenPosition);
		}
	}

	if (ParentSizeBox)
	{
		ParentSizeBox->SetVisibility(bIsOnScreen ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
	}
}
