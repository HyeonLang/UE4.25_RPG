#include "CItemManager.h"
#include "Global.h"
#include "Net/UnrealNetwork.h"

#include "Components/CAbilitySystemComponent.h"

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

FConsumableItemInfo UCItemManager::GetConsumableItemInfoByID(FName ItemID) const
{
    FConsumableItemInfo ReturnInfo;
    if (ConsumableItemDataTable)
    {
        if (ConsumableItemDataTable->FindRow<FConsumableItemInfo>(ItemID, TEXT("Finding Item in Inventory")))
        {
            ReturnInfo = *(ConsumableItemDataTable->FindRow<FConsumableItemInfo>(ItemID, TEXT("Finding Item in Inventory")));
        }
        else
        {
            ReturnInfo = FConsumableItemInfo();
            CLog::Print("Not ConsumableItem Found");
        }
    }
    else
    {
        ReturnInfo = FConsumableItemInfo();
    }
    return ReturnInfo;
}

FItemInfo UCItemManager::GetItemInfoByName(FName ItemName) const
{
    return FItemInfo();
}

bool UCItemManager::UseConsumableItem(AActor* InstigatorActor, FName ItemID, int32 UseItemCount)
{
    if (!InstigatorActor->HasAuthority()) return false;

    FConsumableItemInfo ConsumableItemInfo = GetConsumableItemInfoByID(ItemID);
    if (ConsumableItemInfo.ItemID != ItemID) return false;

    UCAbilitySystemComponent* InstigatorAbilitySystemComp;

    switch (ConsumableItemInfo.ConsumableItemType)
    {
    case EConsumableItemType::Potion:
        std::cout << "Potion item selected!" << std::endl;

        InstigatorAbilitySystemComp = Cast<UCAbilitySystemComponent>(InstigatorActor->GetComponentByClass(UCAbilitySystemComponent::StaticClass()));

        if (InstigatorAbilitySystemComp)
        {
            InstigatorAbilitySystemComp->ApplyHealthChange(InstigatorActor, ConsumableItemInfo.Delta);
        }

        break;

    case EConsumableItemType::Max:
        std::cout << "Invalid item selected!" << std::endl;
        break;

    default:
        std::cout << "Unknown item type!" << std::endl;
        break;
    }

    
    return true;
}


void UCItemManager::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UCItemManager, ItemDataTable);
    DOREPLIFETIME(UCItemManager, ConsumableItemDataTable);
}