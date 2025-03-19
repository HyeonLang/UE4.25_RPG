// Fill out your copyright notice in the Description page of Project Settings.


#include "CLobbyPlayerController.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Character.h"
#include "Game/CGameInstance.h"
#include "Game/CLobbyGameMode.h"
#include "Character/CLobbyCharacter.h"
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
		FUserInfo UserInfo = GI->GetUserInfo();
		
		ServerSpawnLobbyCharacter(UserInfo);
	}

}

void ACLobbyPlayerController::ServerSpawnLobbyCharacter_Implementation(FUserInfo InUserInfo)
{
	ACLobbyGameMode* GM = Cast<ACLobbyGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	float LocationY = (GM->GetNumberOfPlayers() * 150.f) - 300.f;
	FVector Location = FVector(0, LocationY, 0);
	FTransform FM = FTransform();
	FM.SetLocation(Location);
	FM.SetRotation(FRotator(0, -180, 0).Quaternion());

	if (LobbyCharacterClassList.IsValidIndex(InUserInfo.UserMainCharacterIndex))
	{
		LobbyCharacter = GetWorld()->SpawnActorDeferred<ACLobbyCharacter>(LobbyCharacterClassList[InUserInfo.UserMainCharacterIndex], FM, this);
		LobbyCharacter->FinishSpawning(FM);
		LobbyCharacter->SetUserName(InUserInfo.UserName);
	}
}




