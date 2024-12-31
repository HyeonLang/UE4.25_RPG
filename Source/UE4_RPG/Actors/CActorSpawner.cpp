#include "CActorSpawner.h"
#include "Net/UnrealNetwork.h"

#include "Game/CCooldownManager.h"

ACActorSpawner::ACActorSpawner()
{
 	
	PrimaryActorTick.bCanEverTick = true;
	SetReplicates(true);
	SetReplicateMovement(true);
}


void ACActorSpawner::BeginPlay()
{
	Super::BeginPlay();

	SpawnCooldownManager = NewObject<UCCooldownManager>(this);
}


void ACActorSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (SpawnCooldownManager)
	{
		SpawnCooldownManager->CooldownTick(DeltaTime);
	}
}

void ACActorSpawner::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACActorSpawner, SpawnCooldownManager);
}