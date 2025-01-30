// Fill out your copyright notice in the Description page of Project Settings.


#include "CSpawnTargetPoint.h"
#include "CActorSpawner.h"
#include "Global.h"

void ACSpawnTargetPoint::Init(ACActorSpawner* InOwnerSpawner)
{
	OwnerSpawner = InOwnerSpawner;
}

void ACSpawnTargetPoint::SpawnActorAtPoint()
{
	if (SpawnActorClass)
	{
		FTransform TM = GetActorTransform();
		SpawnedActor = GetWorld()->SpawnActorDeferred<AActor>(SpawnActorClass, TM);
		SpawnedActor->FinishSpawning(TM);
		SpawnedActor->SetOwner(this);
	}
}
