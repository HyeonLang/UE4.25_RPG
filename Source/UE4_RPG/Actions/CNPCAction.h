// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"

#include "CActionData.h"
#include "Interfaces/CInitializable.h"
#include "Interfaces/CIconInterface.h"

#include "CNPCAction.generated.h"

class UCNPCActionComponent;
class UCActionData;
class UCCooldownManager;
class ACharacter;

USTRUCT()
struct FNPCActionRepData
{
	GENERATED_BODY()

public:
	UPROPERTY()
	bool bIsRunning;

	UPROPERTY()
	AActor* Instigator;
};

UCLASS()
class UE4_RPG_API UCNPCAction : public UObject, public ICInitializable, public ICIconInterface
{
	GENERATED_BODY()

public:
	UCNPCAction();

	// Object류 클래스 리플리케이트 준비작업
	// 액터가 아닌 오브젝트상속 클래스의 리플리케이트를 도와줌
	FORCEINLINE virtual bool IsSupportedForNetworking() const override
	{
		return true;
	}

public:
	virtual void Initialize() override;

	virtual TSoftObjectPtr<UTexture2D> GetIcon_Implementation() const override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Tick")
	void NPCActionTick(float DeltaTime);

public:
	UFUNCTION(BlueprintNativeEvent, Category = "NPCAction")
	void StartNPCAction(AActor* Instigator);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "NPCAction")
	void StopNPCAction(AActor* Instigator);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "NPCAction")
	void Attack_Elapsed(ACharacter* InstigatorCharacter, int32 AttackIndex = 0);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "NPCAction")
	void Attack_ElapsedByOverlapEvent(ACharacter* InstigatorCharacter, AActor* InstigatorActor, const FHitResult& HitResult, int32 AttackIndex = 0);

	UFUNCTION(BlueprintNativeEvent, Category = "NPCAction")
	bool CanStart(AActor* Instigator, FString& OutMsg);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "NPCAction")
	void StartCooldown(float BaseCooldown);

	UFUNCTION(BlueprintCallable, Category = "NPCAction")
	bool IsRunning() const;

public:
	void SetOwningComponent(UCNPCActionComponent* NewNPCActionComp);

	virtual void SetNPCActionDatas();

	UWorld* GetWorld() const override;

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Action")
	bool GetAimTargetDirection(FRotator& OutDirection, AActor* OutTarget, const bool InIsBossMode);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Action")
	bool SetAimTargetLocation(AActor* InTarget, const bool InIsBossMode);

	UFUNCTION(BlueprintCallable, Category = "Action")
	UCNPCActionComponent* GetOwningComponent() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Montage")
	void PlayMontageDataNPCAction(FActionMontageData MontageData, ACharacter* Instigator, bool bBindEndedDelegate = false);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Montage")
	void PlayMontageNPCAction(UAnimMontage* Montage, ACharacter* Instigator, bool bBindEndedDelegate = false);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Montage")
	void InterruptedNPCAction();

protected:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Montage")
	void BindOnMontageEndDelegate(UAnimMontage* Montage, ACharacter* Instigator);

	UFUNCTION()
	void OnMontageEnd(UAnimMontage* Montage, bool bInterrupted);

public:
	UPROPERTY(BlueprintReadOnly, Replicated, VisibleAnywhere, Category = "Cooldown")
	UCCooldownManager* CooldownManager;

	UPROPERTY(EditDefaultsOnly, Category = "Action")
	FName NPCActionName;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Combo")
	FName NextComboNPCActionName;

	UPROPERTY(BlueprintReadWrite, Category = "Combo")
	FName CurrentComboNPCActionName;

	UPROPERTY(EditDefaultsOnly, Category = "Action")
	bool bAutoStart;

	UPROPERTY(BlueprintReadWrite, Category = "Action")
	TArray<AActor*> IgnoreActors;


protected:
	// Gameplay tags
	UPROPERTY(EditDefaultsOnly, Category = "GameplayTag")
	FGameplayTagContainer GrantTags;

	UPROPERTY(EditDefaultsOnly, Category = "GameplayTag")
	FGameplayTagContainer BlockedTags;


	//Rep
	UPROPERTY(ReplicatedUsing = "OnRep_RepData")
	FNPCActionRepData RepData;

	UFUNCTION()
	void OnRep_RepData();

	UPROPERTY(Replicated)
	UCNPCActionComponent* NPCActionComp;

	UPROPERTY(Replicated)
	float TimeStarted;

protected:
	//Icon
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "UI")
	TSoftObjectPtr<UTexture2D> Icon;		// soft 래퍼런스

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Datas")
	UCActionData* NPCActionDataAssets;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated, Category = "Datas")
	TArray<FActionData> NPCActionDatas;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Combo")
	int32 ComboIndex;
};
