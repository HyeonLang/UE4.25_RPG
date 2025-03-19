// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Game/CGameInstance.h"
#include "CLobbyPlayerController.generated.h"

class ACLobbyCharacter;
/**
 * 
 */
UCLASS()
class UE4_RPG_API ACLobbyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ACLobbyPlayerController();

protected:
	virtual void BeginPlay() override;

public:


protected:
	UFUNCTION(Reliable, Server)
	void ServerSpawnLobbyCharacter(FUserInfo InUserInfo);

	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<ACLobbyCharacter>> LobbyCharacterClassList;

	ACLobbyCharacter* LobbyCharacter;
};
