#include "CLoginMenuWidget.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/PanelWidget.h"
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"
#include "CSessionRowWidget.h"

UCLoginMenuWidget::UCLoginMenuWidget()
{
	ConstructorHelpers::FClassFinder<UUserWidget> SessionRowClassAsset(TEXT("/Game/UI/WB_SessionRow"));

	if (SessionRowClassAsset.Succeeded())
	{
		SessionRowClass = SessionRowClassAsset.Class;
	}
}

bool UCLoginMenuWidget::Initialize()
{
	bool bSuccess = Super::Initialize();
	if (bSuccess == false)
	{
		return false;
	}

	if (LoginButton)
	{
		LoginButton->OnClicked.AddDynamic(this, &UCLoginMenuWidget::LoginServer);
	}


	if (MainCancelButton)
	{
		MainCancelButton->OnClicked.AddDynamic(this, &UCLoginMenuWidget::SwitchLoginMenu);
	}

	if (MainHostButton)
	{
		MainHostButton->OnClicked.AddDynamic(this, &UCLoginMenuWidget::SwitchHostMenu);
	}

	if (MainJoinButton)
	{
		MainJoinButton->OnClicked.AddDynamic(this, &UCLoginMenuWidget::SwitchJoinMenu);
	}


	if (HostCancelButton)
	{
		HostCancelButton->OnClicked.AddDynamic(this, &UCLoginMenuWidget::SwitchMainMenu);
	}

	if (HostServerButton)
	{
		HostServerButton->OnClicked.AddDynamic(this, &UCLoginMenuWidget::HostServer);
	}


	if (JoinCancelButton)
	{
		JoinCancelButton->OnClicked.AddDynamic(this, &UCLoginMenuWidget::SwitchMainMenu);
	}

	if (JoinJoinButton)
	{
		JoinJoinButton->OnClicked.AddDynamic(this, &UCLoginMenuWidget::JoinServer);
	}


	if (QuitButton)
	{
		QuitButton->OnClicked.AddDynamic(this, &UCLoginMenuWidget::QuitPressed);
	}


	return true;
}

void UCLoginMenuWidget::LoginServer()
{
	SwitchMainMenu();
}

void UCLoginMenuWidget::HostServer()
{
	ensure(OwningInstance);

	OwningInstance->Host(DesiredSessionName->GetText().ToString());
}

void UCLoginMenuWidget::JoinServer()
{
	if (SelectedIndex.IsSet() && OwningInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("SelectedIndex : %d"), SelectedIndex.GetValue());

		OwningInstance->Join(SelectedIndex.GetValue());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SelectedIndex is not set yet"));
	}
}

void UCLoginMenuWidget::SwitchLoginMenu()
{
	ensure(MenuSwitcher);
	ensure(LoginMenu);

	MenuSwitcher->SetActiveWidget(LoginMenu);
}

void UCLoginMenuWidget::SwitchJoinMenu()
{
	ensure(MenuSwitcher);
	ensure(JoinMenu);
	ensure(OwningInstance);

	OwningInstance->StartFindSession();

	MenuSwitcher->SetActiveWidget(JoinMenu);
}

void UCLoginMenuWidget::SwitchMainMenu()
{
	ensure(MenuSwitcher);
	ensure(MainMenu);

	MenuSwitcher->SetActiveWidget(MainMenu);
}

void UCLoginMenuWidget::SwitchHostMenu()
{
	ensure(MenuSwitcher);
	ensure(HostMenu);

	MenuSwitcher->SetActiveWidget(HostMenu);
}

void UCLoginMenuWidget::QuitPressed()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	APlayerController* PC = World->GetFirstPlayerController();
	if (PC)
	{
		PC->ConsoleCommand("Quit");
	}
}

void UCLoginMenuWidget::SetSessionList(TArray<FSessionData> InSessionDatas)
{
	SessionList->ClearChildren();

	uint32 i = 0;
	for (const auto& SessionData : InSessionDatas)
	{
		UCSessionRowWidget* SessionRow = CreateWidget<UCSessionRowWidget>(this, SessionRowClass);

		if (SessionRow)
		{
			SessionRow->SessionName->SetText(FText::FromString(SessionData.Name));
			SessionRow->HostUser->SetText(FText::FromString(SessionData.HostUserName));

			FString FractionStr = FString::Printf(TEXT("%d/%d"), SessionData.CurrentPlayers, SessionData.MaxPlayers);
			SessionRow->ConnectionFraction->SetText(FText::FromString(FractionStr));

			SessionRow->Setup(this, i++);
			SessionList->AddChild(SessionRow);
		}
	}
}

void UCLoginMenuWidget::SetSelectedIndex(uint32 InIndex)
{
	SelectedIndex = InIndex;

	for (int32 i = 0; i < SessionList->GetChildrenCount(); i++)
	{
		UCSessionRowWidget* SessionRow = Cast<UCSessionRowWidget>(SessionList->GetChildAt(i));
		if (SessionRow)
		{
			SessionRow->bEverClicked = (SelectedIndex.IsSet() && SelectedIndex.GetValue() == i);
		}
	}
}
