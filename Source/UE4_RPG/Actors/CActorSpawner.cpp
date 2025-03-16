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
	bIsAllRespawnMode = true;
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
			bool bAliveCheck = false;
			for (auto* SpawnTargetPoint : SpawnTargetPoints)
			{
				if (bIsAllRespawnMode)
				{
					if (SpawnTargetPoint->GetSpawnedActor() && !SpawnTargetPoint->GetSpawnedActor()->IsPendingKill())
					{
						bAliveCheck = true;
						break;
					}
				}
				else
				{
					if (!SpawnTargetPoint->GetSpawnedActor() || SpawnTargetPoint->GetSpawnedActor()->IsPendingKill())
					{
						SpawnCooldownManager->StartCooldown(RespawnCooldown);
						break;
					}
				}
			}

			if (bIsAllRespawnMode && !bAliveCheck)
			{
				SpawnCooldownManager->StartCooldown(RespawnCooldown);
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
		if (!SpawnTargetPoint->GetSpawnedActor()) SpawnTargetPoint->SpawnActorAtPoint();
	}
}

void ACActorSpawner::OnCooldownComplete_Implementation(UCCooldownManager* CooldownManager)
{
	SpawnActorsAtTargetPoints();
}
