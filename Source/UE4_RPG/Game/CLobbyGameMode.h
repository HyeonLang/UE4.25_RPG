// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CLobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class UE4_RPG_API ACLobbyGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	ACLobbyGameMode();
public:
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;


private:
	void StartGameStartTimer();
	void StartGame();

protected:
	UPROPERTY(EditDefaultsOnly)
	uint32 MaxNumberOfPlayers;

private:
	uint32 NumberOfPlayers;
	FTimerHandle GameStartTimerHandle;
	
};
