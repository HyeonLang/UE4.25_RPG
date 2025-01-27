#include "CSessionRowWidget.h"
#include "CLoginMenuWidget.h"
#include "Components/Button.h"


bool UCSessionRowWidget::Initialize()
{
	bool bReturn = Super::Initialize();

	bIsTitle = false;
	bEverClicked = false;

	return bReturn && true;
}


void UCSessionRowWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (bIsTitle) return;
	if (bEverClicked)
	{
		if (IsHovered())
		{
			SetColorAndOpacity(FLinearColor::Green);
		}
		else
		{
			SetColorAndOpacity(FLinearColor::Yellow);
		}
	}
	else
	{
		if (IsHovered())
		{
			SetColorAndOpacity(FLinearColor::White);
		}
		else
		{
			SetColorAndOpacity(FLinearColor::Gray);
		}
	}

	
}


void UCSessionRowWidget::Setup(UCLoginMenuWidget* InParentWidget, uint32 InSelfIndex)
{
	ParentWidget = InParentWidget;
	SelfIndex = InSelfIndex;

	RowButton->OnClicked.AddDynamic(this, &UCSessionRowWidget::OnClicked);
}

void UCSessionRowWidget::OnClicked()
{
	ParentWidget->SetSelectedIndex(SelfIndex);
}
