#include "CInventory.h"
#include "Net/UnrealNetwork.h"


UCInventory::UCInventory()
{

}


//void UCInventory::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
//{
//	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
//
//	//DOREPLIFETIME(UCInventory, RepData);
//
//}


void UCInventory::PostInitProperties()
{
	Super::PostInitProperties();

	UCItemManager::GetInstance();
}

// Todo. DB connect
bool UCInventory::AddItem(int32 AddItemID, EItemType AddItemType, int32 AddItemCount)
{
	//아이템 번호 검사 (매니저에서)
	if (AddItemCount <= 0) return false;

	if (InventoryItemList[AddItemType].Contains(AddItemID))
	{
		InventoryItemList[AddItemType][AddItemID] = InventoryItemList[AddItemType][AddItemID] + AddItemCount;
	}
	else
	{
		InventoryItemList[AddItemType].Add(AddItemID, AddItemCount);
	}

	return false;
}


// Todo. DB connect
bool UCInventory::RemoveItem(int32 RemoveItemID, EItemType RemoveItemType, int32 RemoveItemCount)
{
	//아이템 번호 검사 (매니저에서)
	if (RemoveItemCount <= 0) return false;

	if (InventoryItemList[RemoveItemType].Contains(RemoveItemID))
	{
		if (InventoryItemList[RemoveItemType][RemoveItemID] - RemoveItemCount < 0) return false;

		InventoryItemList[RemoveItemType][RemoveItemID] = InventoryItemList[RemoveItemType][RemoveItemID] - RemoveItemCount;

		if (InventoryItemList[RemoveItemType][RemoveItemID] == 0)
		{
			InventoryItemList[RemoveItemType].Remove(RemoveItemID);
		}
	}
	else
	{
		InventoryItemList[RemoveItemType].Add(RemoveItemID, RemoveItemCount);
	}

	return true;
}
