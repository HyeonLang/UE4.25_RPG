// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "CSpawnTargetPoint.generated.h"

class ACActorSpawner;

UCLASS()
class UE4_RPG_API ACSpawnTargetPoint : public ATargetPoint
{
	GENERATED_BODY()
	
public:
	void Init(ACActorSpawner* InOwnerSpawner);

	UFUNCTION(BlueprintCallable)
	void SpawnActorAtPoint();

	UFUNCTION(BlueprintCallable)
	FORCEINLINE AActor* GetSpawnedActor() const { return SpawnedActor; };

public:
	UPROPERTY(BlueprintReadWrite)
	ACActorSpawner* OwnerSpawner;


protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<AActor> SpawnActorClass;

	UPROPERTY(BlueprintReadOnly)
	AActor* SpawnedActor;
};
