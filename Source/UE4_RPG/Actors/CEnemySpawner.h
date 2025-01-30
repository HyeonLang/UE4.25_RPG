// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/CActorSpawner.h"
#include "CEnemySpawner.generated.h"

/**
 * 
 */
UCLASS()
class UE4_RPG_API ACEnemySpawner : public ACActorSpawner
{
	GENERATED_BODY()

public:
	ACEnemySpawner();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION(BlueprintCallable) 
	bool IsAttackMode() const;

	UFUNCTION(BlueprintCallable) 
	AActor* FindNearestTarget(AActor* InActor);

protected:
	TArray<AActor*> TraceMultiForTargets(float Radius);

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float AttackModeRange;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float AttackModeReleaseRange;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TArray<TEnumAsByte<EObjectTypeQuery>> TargetObjectTypes;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TArray<AActor*> TargetActorIgnores;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TArray<AActor*> TargetActors;

	bool bIsAttackMode;

	
};
