#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CActorSpawner.generated.h"

class UCCooldownManager;
class ACSpawnTargetPoint;

UCLASS()
class UE4_RPG_API ACActorSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	ACActorSpawner();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void SpawnActorsAtTargetPoints();
	void SpawnActorsAtRewardPoints();

protected:
	UFUNCTION(BlueprintNativeEvent)
	void OnCooldownComplete(UCCooldownManager* CooldownManager);

protected:
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly)
	TArray<ACSpawnTargetPoint*> SpawnTargetPoints;

	UPROPERTY(BlueprintReadOnly, EditInstanceOnly)
	TArray<ACSpawnTargetPoint*> SpawnRewardPoints;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float RespawnCooldown;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsAllRespawnMode;

public:
	UPROPERTY(BlueprintReadOnly, Replicated, Category = "Cooldown")
	UCCooldownManager* SpawnCooldownManager;

	

};
