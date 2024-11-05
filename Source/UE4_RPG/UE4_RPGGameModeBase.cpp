#include "UE4_RPGGameModeBase.h"

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
	NewPlayer->SetViewTarget(NewPlayer->PlayerCameraActor);
}

void AUE4_RPGGameModeBase::SpawnAndPossessCharacters(ACPlayerController* NewPlayer)
{
	if (!NewPlayer) return;

	NewPlayer->SpawnPlayerCharacter(StartSpawnTransform);
	NewPlayer->SpawnCameraActor(StartSpawnTransform);


	if (NewPlayer->GetPlayerCharacters()[PlayerCharacterStartIndex])
	{

		NewPlayer->PossessCharacter(Cast<ACPlayerCharacter>(NewPlayer->GetPlayerCharacters()[PlayerCharacterStartIndex]), EChangeMode::None);
	}
	
}
