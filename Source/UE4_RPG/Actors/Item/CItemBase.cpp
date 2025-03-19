#include "CItemBase.h"
#include "Net/UnrealNetwork.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"

#include "Global.h"
#include "Actors/Item/CItemManager.h"
#include "Character/CPlayerCharacter.h"
#include "Player/CPlayerController.h"
#include "Game/CPlayerState.h"
#include "Player/CInventory.h"


ACItemBase::ACItemBase()
{
	PrimaryActorTick.bCanEverTick = true;
	SetReplicates(true);
	SetReplicateMovement(true);

	CHelpers::CreateSceneComponent<UStaticMeshComponent>(this, &MeshComp, "MeshComp", RootComponent);
	CHelpers::CreateSceneComponent<USphereComponent>(this, &SphereComp, "SphereComp", MeshComp);
	CHelpers::CreateSceneComponent<UParticleSystemComponent>(this, &EffectComp, "EffectComp", MeshComp);

	SphereComp->SetSphereRadius(50.f);
	MeshComp->SetSimulatePhysics(true);
}

void ACItemBase::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		FItemInfo ItmeInfo = UCItemManager::GetInstance()->GetItemInfoByID(SpawnItemID);
		MeshComp->SetStaticMesh(ItmeInfo.ItemMesh);
		SetItemMesh_NetMulticast(ItmeInfo.ItemMesh);
	}

	if (!FMath::IsNearlyZero(LifeTime))
	{
		SetLifeSpan(LifeTime);
	}
}

void ACItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACItemBase::Interact_Implementation(APawn* InstigatorPawn)
{
	//ACPlayerCharacter* PlayerCharacter = Cast<ACPlayerCharacter>(InstigatorPawn);
	
	ACPlayerController* PlayerController = Cast<ACPlayerController>(InstigatorPawn->GetController());
	if (PlayerController)
	{
		ACPlayerState* PlayerState = PlayerController->GetPlayerState<ACPlayerState>();
		if (PlayerState)
		{
			UCInventory* PlayerInventory = PlayerState->GetPlayerInventory();
			if (!PlayerInventory) return;
			FItemInfo ItmeInfo = UCItemManager::GetInstance()->GetItemInfoByID(SpawnItemID);
			PlayerInventory->AddItem(ItmeInfo.ItemID, ItmeInfo.ItemType, 1);
			
			Destroy();
		}
	}
	
}

FName ACItemBase::GetInteractName_Implementation(APawn* InstigatorPawn)
{
	return UCItemManager::GetInstance()->GetItemInfoByID(SpawnItemID).ItemName;
}

void ACItemBase::SetItemMesh_NetMulticast_Implementation(UStaticMesh* ItemMesh)
{
	MeshComp->SetStaticMesh(ItemMesh);
}

