#include "CItemManager.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"
#include "Particles/ParticleSystem.h"
#include "Components/CAbilitySystemComponent.h"


// ΩÃ±€≈Ê ¿ŒΩ∫≈œΩ∫ √ ±‚»≠
UCItemManager* UCItemManager::Instance = nullptr;

void UCItemManager::Initialize()
{
    CHelpers::GetAssetDynamic(&ItemDataTable, TEXT("/Game/Data/DT_ItemInfo"));
    CHelpers::GetAssetDynamic(&ConsumableItemDataTable, TEXT("/Game/Data/DT_ConsumableItemInfo"));

    UsingEffects.Add(Cast<UParticleSystem>(StaticLoadObject(UParticleSystem::StaticClass(), nullptr, TEXT("/Game/Effects/P_Heal_LongDistance_Target"))));
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
    FConsumableItemInfo ConsumableItemInfo = GetConsumableItemInfoByID(ItemID);

    if (!InstigatorActor->HasAuthority())
    {
        ACharacter* InstigatorCharacter = Cast<ACharacter>(InstigatorActor);
        switch (ConsumableItemInfo.ConsumableItemType)
        {
        case EConsumableItemType::Potion:
            if (InstigatorCharacter && UsingEffects.IsValidIndex((uint8)EConsumableItemType::Potion) && UsingEffects[(uint8)EConsumableItemType::Potion])
            {
                UGameplayStatics::SpawnEmitterAttached(UsingEffects[(uint8)EConsumableItemType::Potion], InstigatorCharacter->GetMesh());
            }
            break;

        case EConsumableItemType::Max:

            break;

        default:

            break;
        }
    }

    if (InstigatorActor->HasAuthority()) 
    {
        if (ConsumableItemInfo.ItemID != ItemID) return false;

        UCAbilitySystemComponent* InstigatorAbilitySystemComp;

        switch (ConsumableItemInfo.ConsumableItemType)
        {
        case EConsumableItemType::Potion:
            

            InstigatorAbilitySystemComp = Cast<UCAbilitySystemComponent>(InstigatorActor->GetComponentByClass(UCAbilitySystemComponent::StaticClass()));


            if (InstigatorAbilitySystemComp)
            {
                InstigatorAbilitySystemComp->ApplyHealthChange(InstigatorActor, ConsumableItemInfo.Delta, FHitResult());
            }

            break;

        case EConsumableItemType::Max:
            
            break;

        default:
            
            break;
        }

        
        return true;
    }

    return false;
}


void UCItemManager::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UCItemManager, ItemDataTable);
    DOREPLIFETIME(UCItemManager, ConsumableItemDataTable);
    DOREPLIFETIME(UCItemManager, UsingEffects);
}