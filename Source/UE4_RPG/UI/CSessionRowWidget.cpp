#include "CSessionRowWidget.h"
#include "CLoginMenuWidget.h"
#include "Components/Button.h"


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
