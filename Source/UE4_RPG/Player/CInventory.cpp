#include "CInventory.h"
#include "Net/UnrealNetwork.h"

#include "Global.h"
#include "Game/CPlayerState.h"


UCInventory::UCInventory()
{
	ItemListRepData = FItemListRepData();
}

void UCInventory::PostInitProperties()
{
	Super::PostInitProperties();

	UCItemManager::GetInstance();
}

// Todo. DB connect
bool UCInventory::AddItem(FName AddItemID, EItemType AddItemType, int32 AddItemCount)
{
	//������ ��ȣ �˻� (�Ŵ�������)
	if (AddItemCount <= 0) return false;

	if (AddItemList(AddItemID, AddItemType, AddItemCount))
	{
		ItemListRepData.SetItemListRepData(AddItemID, AddItemType, AddItemCount, EInventoryChangeType::Add);
	}
	else
	{
		return false;
	}

	return true;
}


// Todo. DB connect
bool UCInventory::RemoveItem(FName RemoveItemID, EItemType RemoveItemType, int32 RemoveItemCount)
{
	//������ ��ȣ �˻� (�Ŵ�������)
	if (RemoveItemCount <= 0) return false;

	if (RemoveItemList(RemoveItemID, RemoveItemType, RemoveItemCount))
	{
		ItemListRepData.SetItemListRepData(RemoveItemID, RemoveItemType, RemoveItemCount, EInventoryChangeType::Remove);
	}
	else
	{
		return false;
	}

	return true;
}

TMap<FName, int32>& UCInventory::GetItemListByItemType(EItemType ItemType)
{
	ACPlayerState* PlayerState = Cast<ACPlayerState>(GetOuter());

	if (!ensure(PlayerState)) return EmptyList;

	switch (ItemType) {
	case EItemType::Misc:
		return MiscItemList;
		break;
	case EItemType::Consumable:
		return ConsumableItemList;
		break;

	case EItemType::Max:
		break;
	default:
		break;
	}

	UE_LOG(LogTemp, Warning, TEXT("Invalid ItemType: %d"), static_cast<int32>(ItemType));

	return EmptyList;
}

bool UCInventory::AddItemList(FName AddItemID, EItemType AddItemType, int32 AddItemCount)
{
	//������ ��ȣ �˻� (�Ŵ�������)
	if (AddItemCount <= 0) return false;
;
	if (GetItemListByItemType(AddItemType).Contains(AddItemID))
	{
		GetItemListByItemType(AddItemType)[AddItemID] = GetItemListByItemType(AddItemType)[AddItemID] + AddItemCount;
	}
	else
	{
		GetItemListByItemType(AddItemType).Add(AddItemID, AddItemCount);
	}

	return true;
}

bool UCInventory::RemoveItemList(FName RemoveItemID, EItemType RemoveItemType, int32 RemoveItemCount)
{
	//������ ��ȣ �˻� (�Ŵ�������)
	if (RemoveItemCount <= 0) return false;

	if (GetItemListByItemType(RemoveItemType).Contains(RemoveItemID))
	{
		if (GetItemListByItemType(RemoveItemType)[RemoveItemID] - RemoveItemCount < 0) return false;

		GetItemListByItemType(RemoveItemType)[RemoveItemID] = GetItemListByItemType(RemoveItemType)[RemoveItemID] - RemoveItemCount;

		if (GetItemListByItemType(RemoveItemType)[RemoveItemID] == 0)
		{
			GetItemListByItemType(RemoveItemType).Remove(RemoveItemID);
		}
	}
	else
	{
		GetItemListByItemType(RemoveItemType).Add(RemoveItemID, RemoveItemCount);
	}



	return true;
}

void UCInventory::OnRep_ItemListRepData()
{

	switch (ItemListRepData.InventoryChangeType)
	{
	case EInventoryChangeType::Add:
		// ������ ������ ������Ű�� ����
		AddItemList(ItemListRepData.ItemID, ItemListRepData.ItemType, ItemListRepData.ItemCount);

		UE_LOG(LogTemp, Warning, TEXT("Quantity Added"));
		break;

	case EInventoryChangeType::Remove:
		// ������ ������ ���ҽ�Ű�� ����
		RemoveItemList(ItemListRepData.ItemID, ItemListRepData.ItemType, ItemListRepData.ItemCount);

		UE_LOG(LogTemp, Warning, TEXT("Quantity Removed"));
		break;

	case EInventoryChangeType::Max:
		// Max ���� ���� ó�� (���� ���� ó���� �ʿ��� �� ���)
		UE_LOG(LogTemp, Warning, TEXT("Invalid Inventory Change Type"));
		break;

	default:
		UE_LOG(LogTemp, Error, TEXT("Unknown Change Type"));
		break;
	}
	
}




void UCInventory::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCInventory, ItemListRepData);

}
