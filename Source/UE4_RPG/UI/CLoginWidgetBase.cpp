#include "CLoginWidgetBase.h"


void UCLoginWidgetBase::SetOwningInstance(ICMenuInterface* InOwningInstance)
{
	OwningInstance = InOwningInstance;
}

void UCLoginWidgetBase::SetInputToUI()
{
	AddToViewport();

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	APlayerController* PC = World->GetFirstPlayerController();
	if (PC)
	{
		FInputModeUIOnly InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

		PC->SetInputMode(InputMode);
		PC->bShowMouseCursor = true;
	}

}

void UCLoginWidgetBase::SetInputToGame()
{
	RemoveFromParent();

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	APlayerController* PC = World->GetFirstPlayerController();
	if (PC)
	{
		FInputModeGameOnly InputMode;
		PC->SetInputMode(InputMode);
		PC->bShowMouseCursor = false;
	}
}
