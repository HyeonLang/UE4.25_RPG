
#include "CLobbyGameMode.h"
#include "TimerManager.h"
#include "Global.h"
#include "CGameInstance.h"

ACLobbyGameMode::ACLobbyGameMode()
{
	DefaultPawnClass = nullptr;

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Character/PlayerCharacters/ChangLi/BP_LobbyCharacter_ChangLi"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		SpawnClass = PlayerPawnBPClass.Class;
	}

	
}

void ACLobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	++NumberOfPlayers;

	CLog::LogOnScreen(this, "Current Players : " + FString::FromInt(NumberOfPlayers));
	UE_LOG(LogTemp, Warning, TEXT("PostLogin : Current Players : %s"), *FString::FromInt(NumberOfPlayers));

	if (NumberOfPlayers >= 1)
	{
		CLog::LogOnScreen(this, "Reached Max Players");
		UE_LOG(LogTemp, Error, TEXT("Reached Max Players"));

		StartGameStartTimer();
	}
}

void ACLobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	--NumberOfPlayers;

	UE_LOG(LogTemp, Warning, TEXT("Logout : Current Players : %s"), *FString::FromInt(NumberOfPlayers));
}

void ACLobbyGameMode::BeginPlay()
{
	Super::BeginPlay();

	
}

void ACLobbyGameMode::StartGameStartTimer()
{
	// Todo. ������ ���� �ڵ� : �κ� �÷��̾� ��Ʈ�ѷ� ���� �� Ŭ�� ���� �Է����ε� ���డ���ϰ�
	GetWorldTimerManager().SetTimer(GameStartTimerHandle, this, &ACLobbyGameMode::StartGame, 5.f);
}

void ACLobbyGameMode::StartGame()
{
	UCGameInstance* GameInstance = Cast<UCGameInstance>(GetGameInstance());
	if (GameInstance)
	{
		GameInstance->StartSession();
	}

	UWorld* World = GetWorld();
	if (World)
	{
		//bUseSeamlessTravel = true;
		World->ServerTravel("/Game/Maps/Demo?listen"); // ���� �� �̵�
	}
}
