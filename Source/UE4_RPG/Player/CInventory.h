// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Actors/Item/CItemManager.h"
#include "CInventory.generated.h"

USTRUCT(BlueprintType)
struct FInventoryItemList
{
	GENERATED_BODY()
public:
	// �⺻ ������
	FInventoryItemList() {}

	// [] ������ �����ε�
	TMap<int32, int32>& operator[](EItemType ItemType)
	{
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

protected:
	TMap<int32, int32> MiscItemList;
	TMap<int32, int32> ConsumableItemList;

	TMap<int32, int32> EmptyList;
};

UCLASS()
class UE4_RPG_API UCInventory : public UObject
{
	GENERATED_BODY()

public:
	UCInventory();

	//// Object�� Ŭ���� ���ø�����Ʈ �غ��۾�
	//// ���Ͱ� �ƴ� ������Ʈ��� Ŭ������ ���ø�����Ʈ�� ������
	//FORCEINLINE virtual bool IsSupportedForNetworking() const override
	//{
	//	return true;
	//}

	virtual void PostInitProperties() override;

public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE FInventoryItemList GetInventoryItemList() const { return InventoryItemList; };

public:
	UFUNCTION(BlueprintCallable)
	bool AddItem(int32 AddItemID, EItemType AddItemType, int32 AddItemCount);

	UFUNCTION(BlueprintCallable)
	bool RemoveItem(int32 RemoveItemID, EItemType RemoveItemType, int32 RemoveItemCount = 1);

protected:
	FInventoryItemList InventoryItemList;
	
};
