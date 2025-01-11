#include "CItemManager.h"
#include "Global.h"

// �̱��� �ν��Ͻ� �ʱ�ȭ
UCItemManager* UCItemManager::Instance = nullptr;

void UCItemManager::Initialize()
{
    CHelpers::GetAssetDynamic(&ItemDataTable, TEXT("/Game/Data/DT_ItemInfo"));
    CHelpers::GetAssetDynamic(&ConsumableItemDataTable, TEXT("/Game/Data/DT_ItemInfo"));
}

UCItemManager* UCItemManager::GetInstance()
{
    if (Instance == nullptr)
    {
        Instance = NewObject<UCItemManager>();
        if (Instance)
        {
            Instance->Initialize();
        }
    }
    return Instance;
}

FItemInfo UCItemManager::GetItemInfoByID(int32 ItemID) const
{
    if (ItemDataTable)
    {
        //// ������ ���̺��� �ش� ������ ID�� �´� ���� ã��
        //TArray<FName> RowNames = ItemDataTable->GetRowNames();
        //for (FName RowName : RowNames)
        //{

        //}
    }
    return FItemInfo();
}

FItemInfo UCItemManager::GetItemInfoByName(FName ItemName) const
{
    return FItemInfo();
}

bool UCItemManager::UseItem(int32 ItemID)
{
    return true;
}
