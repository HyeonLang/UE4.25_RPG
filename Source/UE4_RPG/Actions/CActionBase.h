#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"

#include "CActionData.h"
#include "Interfaces/CInitializable.h"
#include "Interfaces/CIconInterface.h"

#include "CActionBase.generated.h"

class UCActionData;
class UCCooldownManager;
class ACharacter;

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

UCLASS()
class UE4_RPG_API UCActionBase : public UObject, public ICInitializable, public ICIconInterface
{
	GENERATED_BODY()
public:
	UCActionBase();

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
	void ActionTick(float DeltaTime);


public:
	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StartAction(AActor* Instigator);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Action")
	void StopAction(AActor* Instigator);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Action")
	void Attack_Elapsed(ACharacter* InstigatorCharacter, int32 AttackIndex = 0);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Action")
	void Attack_Explode(ACharacter* InstigatorCharacter, FVector HitLocation, FRotator HitRotation, int32 AttackIndex = 0);


	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Action")
	void Attack_ElapsedByOverlapEvent(ACharacter* InstigatorCharacter, AActor* InstigatorActor, const FHitResult& HitResult, int32 AttackIndex = 0);

	UFUNCTION(BlueprintNativeEvent, Category = "NPCAction")
	bool CanStart(AActor* Instigator, FString& OutMsg);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Action")
	void StartCooldown(float BaseCooldown);

	UFUNCTION(BlueprintCallable, Category = "Action")
	bool IsRunning() const;


public:
	UFUNCTION(BlueprintNativeEvent)
	void SetActionDatas();

	UWorld* GetWorld() const;

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Action")
	bool GetAimTargetLocationAndDirection(FVector& OutTargetLocation, FRotator& OutDirection, AActor* OutTarget, const bool InIsBossMode);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Action")
	bool SetAimTargetLocation(AActor* InTarget, const bool InIsBossMode);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Montage")
	void PlayMontageDataAction(FActionMontageData MontageData, ACharacter* Instigator, bool bBindEndedDelegate = false);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Montage")
	void PlayMontageAction(UAnimMontage* Montage, ACharacter* Instigator, bool bBindEndedDelegate = false);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Montage")
	void InterruptedAction();

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Montage")
	void BindOnMontageEndDelegate(UAnimMontage* Montage, ACharacter* Instigator);


public:
	UPROPERTY(BlueprintReadOnly, Replicated, VisibleAnywhere, Category = "Cooldown")
	UCCooldownManager* CooldownManager;

	UPROPERTY(EditDefaultsOnly, Category = "Action")
	FName ActionName;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Combo")
	FName NextComboActionName;

	UPROPERTY(BlueprintReadWrite, Category = "Combo")
	FName CurrentComboActionName;

	UPROPERTY(EditDefaultsOnly, Category = "Action")
	bool bAutoStart;

	UPROPERTY(BlueprintReadWrite, Category = "Combo")
	bool bCanCancelLoop;

	UPROPERTY(BlueprintReadWrite, Category = "Action")
	TArray<AActor*> IgnoreActors;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated, Category = "Datas")
	TArray<FActionData> ActionDatas;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Combo")
	int32 ComboIndex;

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
	float TimeStarted;

protected:
	//Icon
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "UI")
	TSoftObjectPtr<UTexture2D> Icon;	// soft 래퍼런스

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Datas")
	UCActionData* ActionDataAssets;
};
