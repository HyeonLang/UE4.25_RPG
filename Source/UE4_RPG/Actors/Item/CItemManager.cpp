#include "CItemManager.h"
#include "Global.h"
#include "Net/UnrealNetwork.h"

// ΩÃ±€≈Ê ¿ŒΩ∫≈œΩ∫ √ ±‚»≠
UCItemManager* UCItemManager::Instance = nullptr;

void UCItemManager::Initialize()
{
    CHelpers::GetAssetDynamic(&ItemDataTable, TEXT("/Game/Data/DT_ItemInfo"));
    CHelpers::GetAssetDynamic(&ConsumableItemDataTable, TEXT("/Game/Data/DT_ConsumableItemInfo"));
}

UCItemManager* UCItemManager::GetInstance()
{
    if (Instance == nullptr)
    {
        Instance = NewObject<UCItemManager>();
        Instance->AddToRoot();
        if (Instance)
        {
            Instance->Initialize();
        }
    }
    return Instance;
}

FItemInfo UCItemManager::GetItemInfoByID(FName ItemID) const
{
    FItemInfo ReturnInfo;

    if (ItemDataTable)
    {
        if (ItemDataTable->FindRow<FItemInfo>(ItemID, TEXT("Finding Item in Inventory")))
        {
            ReturnInfo = *(ItemDataTable->FindRow<FItemInfo>(ItemID, TEXT("Finding Item in Inventory")));
        }
        else
        {
            ReturnInfo = FItemInfo();
            CLog::Print("Not Item Found");
        }
    }
    else
    {
        ReturnInfo = FItemInfo();
    }

    return ReturnInfo;
}

FItemInfo UCItemManager::GetItemInfoByName(FName ItemName) const
{
    return FItemInfo();
}

bool UCItemManager::UseItem(FName ItemID)
{
    return true;
}


void UCItemManager::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UCItemManager, ItemDataTable);
    DOREPLIFETIME(UCItemManager, ConsumableItemDataTable);
}