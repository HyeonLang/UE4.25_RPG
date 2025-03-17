// Fill out your copyright notice in the Description page of Project Settings.


#include "CLobbyPlayerController.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Character.h"
#include "Game/CGameInstance.h"
#include "Game/CLobbyGameMode.h"
#include "Global.h"

ACLobbyPlayerController::ACLobbyPlayerController()
{
	bAutoManageActiveCameraTarget = false;
}

void ACLobbyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	UCGameInstance* GI = Cast<UCGameInstance>(GetGameInstance());

	if (IsLocalController())
	{
		float Index = GI->GetUserInfo().UserMainCharacterIndex;
		ServerSpawnLobbyCharacter(Index);
	}

}


void ACLobbyPlayerController::ServerSpawnLobbyCharacter_Implementation(float Index)
{
	ACLobbyGameMode* GM = Cast<ACLobbyGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	float LocationY = (GM->GetNumberOfPlayers() * 150.f) - 300.f;
	FVector Location = FVector(0, LocationY, 0);
	FTransform FM = FTransform();
	FM.SetLocation(Location);
	FM.SetRotation(FRotator(0, -180, 0).Quaternion());

	if (LobbyCharacterClassList.IsValidIndex(Index))
	{
		LobbyCharacter = GetWorld()->SpawnActorDeferred<ACharacter>(LobbyCharacterClassList[Index], FM, this);
		LobbyCharacter->FinishSpawning(FM);
	}
}