
#include "CDelegateWidget.h"
#include "Components/TextBlock.h"
#include "GameFramework/PlayerController.h"

bool UCDelegateWidget::Initialize()
{
	Super::Initialize();

	if (ensure(DescriptionText))
	{
		DescriptionText->SetText(Description);

	}
	return true;
}

void UCDelegateWidget::BroadcastCancelButtonClicked()
{
	if (OnCancelButtonClicked.IsBound())
	{
		OnCancelButtonClicked.Broadcast();
	}
}

void UCDelegateWidget::BroadcastOKButtonClicked()
{
	if (OnOKButtonClicked.IsBound())
	{
		OnOKButtonClicked.Broadcast();
	}
}

void UCDelegateWidget::ClearAllDelegates()
{
	if (OnOKButtonClicked.IsBound())
	{
		OnOKButtonClicked.Clear();
	}
	if (OnCancelButtonClicked.IsBound())
	{
		OnCancelButtonClicked.Clear();
	}
}

void UCDelegateWidget::CreateWidget_Implementation()
{
	GetOwningPlayer()->bShowMouseCursor = true;
	GetOwningPlayer()->SetInputMode(FInputModeUIOnly());
}

void UCDelegateWidget::RemoveWidget_Implementation()
{
	GetOwningPlayer()->SetInputMode(FInputModeGameAndUI());
}
