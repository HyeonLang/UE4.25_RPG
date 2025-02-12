#include "CScreenWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"


void UCScreenWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	FVector2D ScreenPosition;
	bool bIsOnScreen = UGameplayStatics::ProjectWorldToScreen(GetOwningPlayer(), AttachToLocation + WorldOffset, ScreenPosition);

	
	float ViewportScale = UWidgetLayoutLibrary::GetViewportScale(this);
	ScreenPosition /= ViewportScale;

	if (ParentSizeBox)
	{
		ParentSizeBox->SetRenderTranslation(ScreenPosition);
	}
	

	if (ParentSizeBox)
	{
		ParentSizeBox->SetVisibility(bIsOnScreen ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	}
}

void UCScreenWidget::SetText(FText Text, EGameAttributeType InGameAttributeType)
{
	DamageText->SetText(Text);
	const FLinearColor LinearColor = UCFunctionLibrary::GetAttributeLinearColor(InGameAttributeType);
	DamageText->SetColorAndOpacity(FSlateColor(LinearColor));
}
