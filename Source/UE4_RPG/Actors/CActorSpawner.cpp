#include "CActorSpawner.h"
#include "Net/UnrealNetwork.h"

#include "Game/CCooldownManager.h"
#include "CSpawnTargetPoint.h"

ACActorSpawner::ACActorSpawner()
{
 	
	PrimaryActorTick.bCanEverTick = true;
	SetReplicates(true);
	SetReplicateMovement(true);
	RespawnCooldown = 10.f;
}


void ACActorSpawner::BeginPlay()
{
	Super::BeginPlay();

	SpawnCooldownManager = NewObject<UCCooldownManager>(this);
	SpawnCooldownManager->OnCooldownCompleteDelegate.AddDynamic(this, &ACActorSpawner::OnCooldownComplete);

	for (auto* SpawnTargetPoint : SpawnTargetPoints)
	{
		SpawnTargetPoint->Init(this);
	}
	SpawnActorsAtTargetPoints();
}


void ACActorSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (SpawnCooldownManager)
	{
		SpawnCooldownManager->CooldownTick(DeltaTime);
	}
	
	if (HasAuthority())
	{
		if (!SpawnCooldownManager->IsCooldownActive())
		{
			for (auto* SpawnTargetPoint : SpawnTargetPoints)
			{
				if (!SpawnTargetPoint->GetSpawnedActor() || SpawnTargetPoint->GetSpawnedActor()->IsPendingKill())
				{
					SpawnCooldownManager->StartCooldown(RespawnCooldown);
					break;
				}
			}
		}
	}
}

void ACActorSpawner::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACActorSpawner, SpawnCooldownManager);
}

void ACActorSpawner::SpawnActorsAtTargetPoints()
{
	if (!HasAuthority()) return;

	for (auto* SpawnTargetPoint : SpawnTargetPoints)
	{
		SpawnTargetPoint->SpawnActorAtPoint();
	}
}

void ACActorSpawner::OnCooldownComplete_Implementation(UCCooldownManager* CooldownManager)
{
	SpawnActorsAtTargetPoints();
}
