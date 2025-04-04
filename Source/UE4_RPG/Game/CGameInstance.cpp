// Fill out your copyright notice in the Description page of Project Settings.


#include "CGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "OnlineSessionSettings.h"
#include "Global.h"
#include "UI/CLoginMenuWidget.h"
#include "Database/CDBManager.h"
#include "CLobbyPlayerController.h"

const static FName SESSION_NAME = TEXT("GameSession");
const static FName SESSION_SETTINGS_KEY = TEXT("ToreKey");

UCGameInstance::UCGameInstance()
{

	ConstructorHelpers::FClassFinder<UUserWidget> MainMenuWidgetClassAsset(TEXT("/Game/UI/WB_LoginMenu"));
	if (MainMenuWidgetClassAsset.Succeeded())
	{
		LoginMenuWidgetClass = MainMenuWidgetClassAsset.Class;
	}

	ConstructorHelpers::FClassFinder<UUserWidget> LoadingWidgetClassAsset(TEXT("/Game/UI/WB_LoadingWidget"));
	if (LoadingWidgetClassAsset.Succeeded())
	{
		LoadingWidgetClass = LoadingWidgetClassAsset.Class;
	}
	//CHelpers::GetClass<UUserWidget>(&InGameMenuWidgetClass, TEXT("/Game/UI/WB_InGameMenu"));
	
}

void UCGameInstance::Init()
{
	Super::Init();

	IOnlineSubsystem* OSS = IOnlineSubsystem::Get();
	if (OSS)
	{
		UE_LOG(LogTemp, Display, TEXT("OSS Name : %s"), *OSS->GetSubsystemName().ToString());

		SessionInterface = OSS->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			UE_LOG(LogTemp, Display, TEXT("Session Interface found"));

			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UCGameInstance::OnCreateSessionCompleted);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UCGameInstance::OnDestroySessionCompleted);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UCGameInstance::OnFindSessionCompleted);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UCGameInstance::OnJoinSessionCompleted);
		}
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("Not found OSS"));
	}

	if (GEngine)
	{
		GEngine->OnNetworkFailure().AddUObject(this, &UCGameInstance::OnNetworkFailure);
	}

	DBManager = NewObject<UCDBManager>();
	if (ensure(DBManager))
	{
		DBManager->OnLoginSuccessEvent.AddDynamic(this, &UCGameInstance::OnLoginSuccessEvent);
	}

	FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &UCGameInstance::OnPreLoadMap);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UCGameInstance::OnPostLoadMapWithWorld);
}

void UCGameInstance::Login(const FString& UserId, const FString& Password)
{
	DBManager->RequestLogin(UserId, Password);
}

void UCGameInstance::Host(FString InDesiredSessionName)
{
	DesiredSessionName = InDesiredSessionName;

	if (SessionInterface.IsValid())
	{
		auto ExsistingSession = SessionInterface->GetNamedSession(SESSION_NAME);

		if (ExsistingSession)
		{
			SessionInterface->DestroySession(SESSION_NAME);
		}
		else
		{
			CreateSession_Internal();
		}
	}
}

void UCGameInstance::CreateSession_Internal()
{
	if (SessionInterface.IsValid())
	{
		FOnlineSessionSettings SessionSettings;

		if (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL")
		{
			SessionSettings.bIsLANMatch = true;
		}
		else
		{
			SessionSettings.bIsLANMatch = false;
		}

		
		SessionSettings.NumPublicConnections = 3;
		SessionSettings.bShouldAdvertise = true;
		SessionSettings.bAllowJoinInProgress = true;
		SessionSettings.bUsesPresence = true;
		SessionSettings.Set(SESSION_SETTINGS_KEY, DesiredSessionName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
		//SessionSettings.Set(,)
		SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);
	}
}

void UCGameInstance::Join(uint32 InIndex)
{
	if (!SessionInterface.IsValid() || !SessionSearch.IsValid())
	{
		return;
	}

	if (LoginMenu)
	{
		LoginMenu->SetInputToGame();
	}

	SessionInterface->JoinSession(0, SESSION_NAME, SessionSearch->SearchResults[InIndex]);
}

void UCGameInstance::OpenLoginMenuLevel()
{
	APlayerController* PC = GetFirstLocalPlayerController();
	if (!PC)
	{
		return;
	}

	PC->ClientTravel("/Game/Maps/LoginMenu", ETravelType::TRAVEL_Absolute);
}

void UCGameInstance::StartFindSession()
{
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	if (SessionSearch.IsValid())
	{
		CLog::LogOnScreen(this, "Start Finding Session");

		SessionSearch->MaxSearchResults = 100;
		SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	}
}

void UCGameInstance::StartSession()
{
	if (SessionInterface.IsValid())
	{
		SessionInterface->StartSession(SESSION_NAME);
	}
}

void UCGameInstance::LoadMainMenu()
{
	ensure(LoginMenuWidgetClass);
	
	LoginMenu = CreateWidget<UCLoginMenuWidget>(this, LoginMenuWidgetClass);
	if (!LoginMenu)
	{
		return;
	}

	LoginMenu->SetOwningInstance(this);

	LoginMenu->SetInputToUI();
}

void UCGameInstance::LoadInGameMenu()
{
	/*ensure(InGameMenuWidgetClass);

	UCWidgetBase* InGameMenu = CreateWidget<UCMenuWidgetBase>(this, InGameMenuWidgetClass);
	if (!InGameMenu)
	{
		return;
	}

	InGameMenu->SetOwningInstance(this);
	InGameMenu->SetInputToUI();*/
}

void UCGameInstance::OnCreateSessionCompleted(FName InSessionName, bool bWasSuccessful)
{
	UE_LOG(LogTemp, Error, TEXT("Created Session"));

	if (!bWasSuccessful)
	{
		CLog::LogOnScreen(this, "Could not create session!", FColor::Red);
		return;
	}

	CLog::LogOnScreen(this, "Create Session Completed, Session Name : " + InSessionName.ToString(), FColor::Green);

	if (LoginMenu)
	{
		LoginMenu->SetInputToGame();
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}
	World->ServerTravel("/Game/Maps/Lobby?listen");
}

void UCGameInstance::OnDestroySessionCompleted(FName InSessionName, bool bWasSuccessful)
{
	UE_LOG(LogTemp, Error, TEXT("Destroied Session"));

	if (bWasSuccessful)
	{
		CreateSession_Internal();
	}
}

void UCGameInstance::OnFindSessionCompleted(bool bWasSuccessful)
{
	if (bWasSuccessful && SessionSearch.IsValid() && LoginMenu)
	{
		CLog::LogOnScreen(this, "Finish Finding Session");

		TArray<FSessionData> SessionList;

		for (const auto& SearchResult : SessionSearch->SearchResults)
		{
			UE_LOG(LogTemp, Error, TEXT("Found Session ID : %s"), *SearchResult.GetSessionIdStr());
			UE_LOG(LogTemp, Error, TEXT("Ping(ms) : %d"), SearchResult.PingInMs);

			FSessionData Data;
			Data.MaxPlayers = SearchResult.Session.SessionSettings.NumPublicConnections;
			Data.CurrentPlayers = Data.MaxPlayers - SearchResult.Session.NumOpenPublicConnections;
			Data.HostUserName = SearchResult.Session.OwningUserName;

			FString SessionName;
			if (SearchResult.Session.SessionSettings.Get(SESSION_SETTINGS_KEY, SessionName))
			{
				Data.Name = SessionName;
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("SessionSettings Key is not valid"));
			}

			SessionList.Add(Data);
		}

		LoginMenu->SetSessionList(SessionList);
	}
}

void UCGameInstance::OnJoinSessionCompleted(FName InSessionName, EOnJoinSessionCompleteResult::Type InResult)
{
	FString Address;
	if (SessionInterface->GetResolvedConnectString(InSessionName, Address) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("Could not resolve to IP address"));
		return;
	}

	CLog::LogOnScreen(this, "Join to " + Address, FColor::Green);

	APlayerController* PC = GetFirstLocalPlayerController();
	if (!PC)
	{
		return;
	}

	PC->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}

void UCGameInstance::OnNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorMessage)
{
	CLog::LogOnScreen(this, ErrorMessage, FColor::Red);
	OpenLoginMenuLevel();
}

void UCGameInstance::OnLoginSuccessEvent(const FString& UserData)
{

	// JSON ���� �Ľ�
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(UserData);

	FJsonSerializer::Deserialize(Reader, JsonObject);

	bool bSuccess = false;
	if (JsonObject->GetStringField("status") == "fail")
	{
		bSuccess = false;
	}
	else
	{
		bSuccess = true;

		InitUserInfo(JsonObject);
	}

	if (LoginMenu)
	{
		LoginMenu->LoginServerResponse(bSuccess);
	}

	CLog::Print(JsonObject->GetStringField("status"), -1, 10.f, FColor::Red);
	CLog::Log(UserData);
}

void UCGameInstance::OnPreLoadMap(const FString& InMapName)
{
	////if (InMapName.Right(4) != "Demo") return;

	//if (!GetWorld()->IsServer())
	//{
	//	if (LoadingWidgetClass)
	//	{
	//		LoadingWidget = CreateWidget<UUserWidget>(GetWorld(), LoadingWidgetClass);
	//		LoadingWidget->AddToViewport();
	//	}
	//}
	
	
}

void UCGameInstance::OnPostLoadMapWithWorld(UWorld* InLoadedWorld)
{
	
}

void UCGameInstance::InitUserInfo(TSharedPtr<FJsonObject> JsonUserData)
{
	UserInfo.UserId = FName(JsonUserData->GetStringField("userid"));
	UserInfo.UserName = FText::FromString(JsonUserData->GetStringField("username"));
	UserInfo.UserMainCharacterIndex = FCString::Atoi(*JsonUserData->GetStringField("main_character_index"));
	
}

