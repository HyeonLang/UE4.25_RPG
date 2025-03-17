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
	virtual void BeginPlay() override;

private:
	void StartGameStartTimer();
	void StartGame();

	UFUNCTION()
	void SpawnLobbyCharacter(APlayerController* NewPlayer);

public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetMaxNumberOfPlayers() const { return MaxNumberOfPlayers; };

	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetNumberOfPlayers() const { return NumberOfPlayers; };

protected:
	UPROPERTY(EditDefaultsOnly)
	int32 MaxNumberOfPlayers;

	UPROPERTY(BlueprintReadWrite)
	int32 NumberOfPlayers;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSubclassOf<AActor> LobbyCameraActorClass;

protected:
	FTimerHandle GameStartTimerHandle;

};
