#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "UE4_RPGGameModeBase.generated.h"

class ACPlayerController;

UCLASS()
class UE4_RPG_API AUE4_RPGGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AUE4_RPGGameModeBase();

protected:
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void BeginPlay() override;
	virtual void StartPlay() override;
protected:
	UFUNCTION(Client, Reliable)
	void ClientSetViewTarget(ACPlayerController* NewPlayer);

private:
	void SpawnAndPossessCharacters(ACPlayerController* NewPlayer);

private:
	UPROPERTY(EditDefaultsOnly, Category = "Character")
	int32 PlayerCharacterStartIndex;

	FTransform StartSpawnTransform;

	ACPlayerController* PC;
};
