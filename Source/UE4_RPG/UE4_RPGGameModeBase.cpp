#include "UE4_RPGGameModeBase.h"

#include "Character/CPlayerCharacter.h"
#include "Player/CPlayerController.h"
#include "Components/CapsuleComponent.h"
#include "Global.h"

AUE4_RPGGameModeBase::AUE4_RPGGameModeBase()
{
	PlayerCharacterStartIndex = 0;
	StartSpawnTransform.SetLocation(FVector(0, 0, 130));
}

void AUE4_RPGGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	SpawnAndPossessCharacters();
}

void AUE4_RPGGameModeBase::SpawnAndPossessCharacters()
{
	ACPlayerController* PlayerController = Cast<ACPlayerController>(GetWorld()->GetFirstPlayerController());
	if (!PlayerController) return;

	PlayerController->SpawnPlayerCharacter(StartSpawnTransform);


	if (PlayerController->GetPlayerCharacters()[PlayerCharacterStartIndex])
	{
		PlayerController->PossessCharacter(Cast<ACPlayerCharacter>(PlayerController->GetPlayerCharacters()[PlayerCharacterStartIndex]), EChangeMode::None);
	}
}
