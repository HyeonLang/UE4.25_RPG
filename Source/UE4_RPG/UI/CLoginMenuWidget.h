
#pragma once

#include "CoreMinimal.h"
#include "CLoginWidgetBase.h"
#include "CLoginMenuWidget.generated.h"

class UButton;
class UWidgetSwitcher;
class UPanelWidget;
class UEditableTextBox;

USTRUCT()
struct FSessionData
{
	GENERATED_BODY()

public:
	FString Name;
	uint16 CurrentPlayers;
	uint16 MaxPlayers;
	FString HostUserName;
};

UCLASS()
class UE4_RPG_API UCLoginMenuWidget : public UCLoginWidgetBase
{
	GENERATED_BODY()

public:
	UCLoginMenuWidget();

protected:
	virtual bool Initialize() override;

private:
	UFUNCTION()
	void LoginServer();

	UFUNCTION()
	void HostServer();

	UFUNCTION()
	void JoinServer();

	UFUNCTION()
	void SwitchLoginMenu();

	UFUNCTION()
	void SwitchJoinMenu();

	UFUNCTION()
	void SwitchMainMenu();

	UFUNCTION()
	void SwitchHostMenu();

	UFUNCTION()
	void QuitPressed();

public:
	void SetSessionList(TArray<FSessionData> InSessionDatas);
	void SetSelectedIndex(uint32 InIndex);

protected:
	//Buttons
	UPROPERTY(meta = (BindWidget))
	UButton* LoginButton;

	UPROPERTY(meta = (BindWidget))
	UButton* MainCancelButton;

	UPROPERTY(meta = (BindWidget))
	UButton* MainHostButton;

	UPROPERTY(meta = (BindWidget))
	UButton* MainJoinButton;

	UPROPERTY(meta = (BindWidget))
	UButton* HostCancelButton;

	UPROPERTY(meta = (BindWidget))
	UButton* HostServerButton;

	UPROPERTY(meta = (BindWidget))
	UButton* JoinCancelButton;

	UPROPERTY(meta = (BindWidget))
	UButton* JoinJoinButton;

	UPROPERTY(meta = (BindWidget))
	UButton* QuitButton;

	//Widgets
	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* MenuSwitcher;

	UPROPERTY(meta = (BindWidget))
	UWidget* LoginMenu;

	UPROPERTY(meta = (BindWidget))
	UWidget* MainMenu;

	UPROPERTY(meta = (BindWidget))
	UWidget* HostMenu;

	UPROPERTY(meta = (BindWidget))
	UWidget* JoinMenu;

	UPROPERTY(meta = (BindWidget))
	UPanelWidget* SessionList;

	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* DesiredUserID;

	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* DesiredPassword;

	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* DesiredSessionName;

private:
	TSubclassOf<UUserWidget> SessionRowClass;
	TOptional<uint32> SelectedIndex;
	
};
