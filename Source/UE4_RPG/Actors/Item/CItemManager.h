#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "Interfaces/CInitializable.h"
#include "CItemManager.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
    Misc UMETA(DisplayName = "Miscellaneous"),
    Consumable UMETA(DisplayName = "Consumable"),
    Max
};

UENUM(BlueprintType)
enum class EConsumableItemType : uint8
{
    Potion UMETA(DisplayName = "Potion"),
    Max
};

USTRUCT(BlueprintType)
struct FItemInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Info")
    int32 ItemID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Info")
    FName ItemName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Info")
    FString ItemDescription;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Info")
    UTexture2D* ItemIcon;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Info")
    EItemType ItemType;
};

USTRUCT(BlueprintType)
struct FConsumableItemInfo : public FTableRowBase
{
    GENERATED_USTRUCT_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Info")
    int32 ItemID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Info")
    EConsumableItemType ConsumableItemType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Info")
    float Delta;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Info")
    float Cooldown;
};


UCLASS()
class UE4_RPG_API UCItemManager : public UObject, public ICInitializable
{
	GENERATED_BODY()

private:
    // �̱��� �ν��Ͻ�
    static UCItemManager* Instance;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data", meta = (AllowPrivateAccess = "true"))
    UDataTable* ItemDataTable;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data", meta = (AllowPrivateAccess = "true"))
    UDataTable* ConsumableItemDataTable;

public:
    virtual void Initialize() override;

public:
    // �̱��� �ν��Ͻ� ��ȯ
    UFUNCTION(BlueprintCallable, Category = "Item")
    static UCItemManager* GetInstance();

    // ������ ������ ������ ���̺��� �������� �Լ�
    UFUNCTION(BlueprintCallable, Category = "Item")
    FItemInfo GetItemInfoByID(int32 ItemID) const;

    // ������ ������ ������ ���̺��� �������� �Լ�
    UFUNCTION(BlueprintCallable, Category = "Item")
    FItemInfo GetItemInfoByName(FName ItemName) const;

    // ������ ��� �Լ�
    UFUNCTION(BlueprintCallable, Category = "Item")
    bool UseItem(int32 ItemID);
	
};
