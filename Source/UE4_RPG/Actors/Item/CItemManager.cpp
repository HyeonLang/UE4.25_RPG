#include "CItemManager.h"
#include "Global.h"

// 싱글톤 인스턴스 초기화
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
        //// 데이터 테이블에서 해당 아이템 ID에 맞는 행을 찾음
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
