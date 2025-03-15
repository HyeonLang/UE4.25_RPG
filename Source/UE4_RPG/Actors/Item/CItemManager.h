#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "Interfaces/CInitializable.h"
#include "CItemManager.generated.h"

class UParticleSystem;

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
    FName ItemID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Info")
    FName ItemName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Info")
    FText ItemDescription;

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
    FName ItemID;

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
    // 싱글톤 인스턴스
    static UCItemManager* Instance;

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated, Category = "Item Data", meta = (AllowPrivateAccess = true))
    UDataTable* ItemDataTable;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated, Category = "Item Data", meta = (AllowPrivateAccess = true))
    UDataTable* ConsumableItemDataTable;

    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Replicated)
    TArray<UParticleSystem*> UsingEffects;

public:
    virtual void Initialize() override;

    // Object류 클래스 리플리케이트 준비작업
    // 액터가 아닌 오브젝트상속 클래스의 리플리케이트를 도와줌
    FORCEINLINE virtual bool IsSupportedForNetworking() const override
    {
        return true;
    }

public:
    // 싱글톤 인스턴스 반환
    UFUNCTION(BlueprintCallable, Category = "Item")
    static UCItemManager* GetInstance();

    // 아이템 정보를 데이터 테이블에서 가져오는 함수
    UFUNCTION(BlueprintCallable, Category = "Item")
    FItemInfo GetItemInfoByID(FName ItemID) const;

    UFUNCTION(BlueprintCallable, Category = "Item")
    FConsumableItemInfo GetConsumableItemInfoByID(FName ItemID) const;

    // 아이템 정보를 데이터 테이블에서 가져오는 함수
    UFUNCTION(BlueprintCallable, Category = "Item")
    FItemInfo GetItemInfoByName(FName ItemName) const;

public:
    UFUNCTION(BlueprintCallable, Category = "ItemUse")
    bool UseConsumableItem(AActor* InstigatorActor, FName ItemID, int32 UseItemCount = 1);

};
