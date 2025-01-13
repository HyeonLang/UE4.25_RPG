// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Actors/Item/CItemManager.h"
#include "CInventory.generated.h"

UENUM(BlueprintType)
enum class EInventoryChangeType : uint8
{
	Add UMETA(DisplayName = "Add Quantity"),
	Remove UMETA(DisplayName = "Remove Quantity"),
	Max
};

USTRUCT(BlueprintType)
struct FItemListRepData
{
	GENERATED_BODY()

public:
	FItemListRepData()
		: ReplicateSwitch(false), ItemID("0"), ItemType(EItemType::Misc), ItemCount(1), InventoryChangeType(EInventoryChangeType::Add)
	{
	}

	void SetItemListRepData(FName NewItemID, EItemType NewItemType, int32 NewItemCount, EInventoryChangeType NewInventoryChangeType)
	{
		this->ItemID = NewItemID;
		this->ItemType = NewItemType;
		this->ItemCount = NewItemCount;
		this->InventoryChangeType = NewInventoryChangeType;

		this->ReplicateSwitch = !this->ReplicateSwitch;
	}

public:
	UPROPERTY()
	bool ReplicateSwitch;

	UPROPERTY()
	FName ItemID;

	UPROPERTY()
	EItemType ItemType;

	UPROPERTY()
	int32 ItemCount;

	UPROPERTY()
	EInventoryChangeType InventoryChangeType;
};

UCLASS()
class UE4_RPG_API UCInventory : public UObject
{
	GENERATED_BODY()

public:
	UCInventory();

	// Object류 클래스 리플리케이트 준비작업
	// 액터가 아닌 오브젝트상속 클래스의 리플리케이트를 도와줌
	FORCEINLINE virtual bool IsSupportedForNetworking() const override
	{
		return true;
	}

	virtual void PostInitProperties() override;


public:
	UFUNCTION(BlueprintCallable)
	bool AddItem(FName AddItemID, EItemType AddItemType, int32 AddItemCount = 1);

	UFUNCTION(BlueprintCallable)
	bool RemoveItem(FName RemoveItemID, EItemType RemoveItemType, int32 RemoveItemCount = 1);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TMap<FName, int32>& GetItemListByItemType(EItemType ItemType);

protected:
	bool AddItemList(FName AddItemID, EItemType AddItemType, int32 AddItemCount = 1);
	bool RemoveItemList(FName RemoveItemID, EItemType RemoveItemType, int32 RemoveItemCount = 1);

public:
	UPROPERTY(BlueprintReadOnly)
	TMap<FName, int32> MiscItemList;

	UPROPERTY(BlueprintReadOnly)
	TMap<FName, int32> ConsumableItemList;

	UPROPERTY(BlueprintReadOnly)
	TMap<FName, int32> EmptyList;

protected:
	UPROPERTY(ReplicatedUsing = "OnRep_ItemListRepData")
	FItemListRepData ItemListRepData;

	UFUNCTION()
	void OnRep_ItemListRepData();

};
