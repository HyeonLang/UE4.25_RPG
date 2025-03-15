
#include "CPlayerState.h"

#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"

#include "Global.h"
#include "Player/CInventory.h"
#include "Player/CPlayerController.h"
#include "Character/CPlayerCharacter.h"


ACPlayerState::ACPlayerState()
{
	PrimaryActorTick.bCanEverTick = true;
}

bool ACPlayerState::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool bChangedSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	
	if (PlayerInventory)
	{
		bChangedSomething |= Channel->ReplicateSubobject(PlayerInventory, *Bunch, *RepFlags);
	}
	

	return bChangedSomething;
}

void ACPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (HasAuthority())
	{
		PlayerInventory = NewObject<UCInventory>(this);
	}
	
}

void ACPlayerState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (PlayerInventory)
	{
		FString b = "false";
		if (HasAuthority())
		{
			b = "true";
		}
		FString Str = "MIsc : " + FString::FromInt(PlayerInventory->MiscItemList.Num()) + "Is Server : " + b;
		CLog::Print(Str, -1, DeltaTime);
	}
	else
	{
		CLog::Print("No INven", -1, DeltaTime);
	}
}

bool ACPlayerState::UseInventoryItem(FName UseItemID, EItemType UseItemType, int32 UseItemCount)
{

	
	ACPlayerController* PlayerController = Cast<ACPlayerController>(GetOwner());
	if (!PlayerController) return false;

	if (PlayerInventory->UseItem(PlayerController->GetPlayerCharacter(), UseItemID, UseItemType, UseItemCount))
	{
		if (!PlayerInventory->RemoveItem(UseItemID, UseItemType, UseItemCount)) return false;

		return true;
	}
	
	if (!HasAuthority())
	{
		CLog::Print("UseItem");
		ServerUseInventoryItem(UseItemID, UseItemType, UseItemCount);

		return true;
	}

	return false;
}

void ACPlayerState::ServerUseInventoryItem_Implementation(FName UseItemID, EItemType UseItemType, int32 UseItemCount)
{
	ACPlayerController* PlayerController = Cast<ACPlayerController>(GetOwner());
	if (!PlayerController) return;

	if (PlayerInventory->UseItem(PlayerController->GetPlayerCharacter(), UseItemID, UseItemType, UseItemCount))
	{
		PlayerInventory->RemoveItem(UseItemID, UseItemType, UseItemCount);
	}
}





void ACPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACPlayerState, PlayerInventory);
}