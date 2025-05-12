#include "UE4_RPGGameModeBase.h"
#include "GameFramework/PlayerStart.h"
#include "EngineUtils.h"
#include "Character/CPlayerCharacter.h"
#include "Player/CPlayerController.h"
#include "Player/CPlayerCameraActor.h"
#include "Components/CapsuleComponent.h"
#include "Global.h"

AUE4_RPGGameModeBase::AUE4_RPGGameModeBase()
{
	PlayerCharacterStartIndex = 0;
	StartSpawnTransform.SetLocation(FVector(0, 0, 130));
}

void AUE4_RPGGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	if (!GetWorld()) return;

	for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
	{
		APlayerStart* PlayerStart = *It;
		StartSpawnTransform = PlayerStart->GetActorTransform();
		
		break;
	}
}

void AUE4_RPGGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	


	ACPlayerController* PlayerController = Cast<ACPlayerController>(NewPlayer);
	SpawnAndPossessCharacters(PlayerController);
	PC = PlayerController;
}

void AUE4_RPGGameModeBase::BeginPlay()
{
	Super::BeginPlay();


}

void AUE4_RPGGameModeBase::StartPlay()
{
	Super::StartPlay();
	ClientSetViewTarget(PC);

	
	
}

void AUE4_RPGGameModeBase::ClientSetViewTarget_Implementation(ACPlayerController* NewPlayer)
{
	if (NewPlayer->PlayerCameraActor)
		NewPlayer->SetViewTarget(NewPlayer->PlayerCameraActor);
}

void AUE4_RPGGameModeBase::SpawnAndPossessCharacters(ACPlayerController* NewPlayer)
{
	if (!NewPlayer) return;

	NewPlayer->SpawnPlayerCharacter(StartSpawnTransform);
	NewPlayer->SpawnCameraActor(StartSpawnTransform);


	if (NewPlayer->GetPlayerCharacters()[PlayerCharacterStartIndex])
	{

		NewPlayer->PossessCharacter(Cast<ACPlayerCharacter>(NewPlayer->GetPlayerCharacters()[PlayerCharacterStartIndex]), FVector(0, 0, 0), EChangeMode::None);
	}
	
}
