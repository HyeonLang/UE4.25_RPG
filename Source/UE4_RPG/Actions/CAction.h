#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"

#include "CActionData.h"

#include "CAction.generated.h"

class UCActionComponent;
class UCActionData;

USTRUCT()
struct FActionRepData
{
	GENERATED_BODY()

public:
	UPROPERTY()
	bool bIsRunning;

	UPROPERTY()
	AActor* Instigator;
};


UCLASS(Blueprintable)
class UE4_RPG_API UCAction : public UObject
{
	GENERATED_BODY()

public:
	UCAction();

	// Object�� Ŭ���� ���ø�����Ʈ �غ��۾�
	// ���Ͱ� �ƴ� ������Ʈ��� Ŭ������ ���ø�����Ʈ�� ������
	FORCEINLINE virtual bool IsSupportedForNetworking() const override
	{
		return true;
	}

public:
	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StartAction(AActor* Instigator);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Action")
	void StopAction(AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	bool CanStart(AActor* Instigator);

	UFUNCTION(BlueprintCallable, Category = "Action")
	bool IsRunning() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Action")
	void GetAimTargetDirection(FRotator& OutDirection, AActor* OutTarget, const bool InIsBossMode);

	void SetOwningComponent(UCActionComponent* NewActionComp);
	void SetActionDatas();

	UWorld* GetWorld() const override;

public:
	UFUNCTION(BlueprintCallable, Category = "Action")
	UCActionComponent* GetOwningComponent() const;

public:
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	FName ActionName;

	UPROPERTY(EditDefaultsOnly, Category = "Action")
	bool bAutoStart;

protected:
	// Gameplay tags
	UPROPERTY(EditDefaultsOnly, Category = "GameplayTag")
	FGameplayTagContainer GrantTags;

	UPROPERTY(EditDefaultsOnly, Category = "GameplayTag")
	FGameplayTagContainer BlockedTags;


	//Rep
	UPROPERTY(ReplicatedUsing = "OnRep_RepData")
	FActionRepData RepData;

	UFUNCTION()
	void OnRep_RepData();

	UPROPERTY(Replicated)
	UCActionComponent* ActionComp;

	UPROPERTY(Replicated)
	float TimeStarted;

protected:
	//Icon
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "UI")
	TSoftObjectPtr<UTexture2D> Icon;		// soft ���۷���

	/*UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Datas")
	TSubclassOf<UCActionData> ActionDataClass;*/


	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Datas")
	UCActionData* ActionDataAssets;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated, Category = "Datas")
	TArray<FActionData> ActionDatas;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Combo")
	int32 ComboIndex;
};
