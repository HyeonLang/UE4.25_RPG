#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "UE4_RPGGameModeBase.generated.h"

UCLASS()
class UE4_RPG_API AUE4_RPGGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AUE4_RPGGameModeBase();

protected:
	virtual void BeginPlay() override;

private:
	void SpawnAndPossessCharacters();

private:
	UPROPERTY(EditDefaultsOnly, Category = "Character")
	int32 PlayerCharacterStartIndex;

	FTransform StartSpawnTransform;
};
