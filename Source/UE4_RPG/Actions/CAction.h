#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"

#include "CActionData.h"
#include "Interfaces/CInitializable.h"
#include "Interfaces/CIconInterface.h"

#include "CAction.generated.h"

class UCActionComponent;
class UCActionData;
class UCCooldownManager;

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
class UE4_RPG_API UCAction : public UObject, public ICInitializable, public ICIconInterface
{
	GENERATED_BODY()

public:
	UCAction();

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
	void Attack_ElapsedByOverlapEvent(ACharacter* InstigatorCharacter, AActor* InstigatorActor, const FHitResult& HitResult, int32 AttackIndex = 0);

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	bool CanStart(AActor* Instigator, FString& OutMsg);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Action")
	void StartCooldown(float BaseCooldown);

	UFUNCTION(BlueprintCallable, Category = "Action")
	bool IsRunning() const;

public:
	UFUNCTION(BlueprintCallable, Category = "Combo")
	void SetCanCombo(bool InNew);

	void SetOwningComponent(UCActionComponent* NewActionComp);

	virtual void SetActionDatas();

	UWorld* GetWorld() const override;


public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Action")
	bool GetAimTargetDirection(FRotator& OutDirection, AActor* OutTarget, const bool InIsBossMode);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Action")
	bool SetAimTargetLocation(AActor* InTarget, const bool InIsBossMode);

	UFUNCTION(BlueprintCallable, Category = "Action")
	UCActionComponent* GetOwningComponent() const;

	UFUNCTION(BlueprintCallable, Category = "Combo")
	FORCEINLINE bool GetCanCombo() const { return bCanCombo; }

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

	UPROPERTY(BlueprintReadWrite, Category = "Action")
	TArray<AActor*> IgnoreActors;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Montage")
	void PlayMontageDataAction(FActionMontageData MontageData, ACPlayerCharacter* Instigator, bool bBindEndedDelegate = false);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Montage")
	void PlayMontageAction(UAnimMontage* Montage, ACPlayerCharacter* Instigator, bool bBindEndedDelegate = false);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Montage")
	void InterruptedAction();


protected:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Montage")
	void BindOnMontageEndedDelegate(UAnimMontage* Montage, ACPlayerCharacter* Instigator);

	UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

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
	TSoftObjectPtr<UTexture2D> Icon;		// soft 래퍼런스

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Datas")
	UCActionData* ActionDataAssets;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated, Category = "Datas")
	TArray<FActionData> ActionDatas;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Combo")
	int32 ComboIndex;

	bool bCanCombo;



	/*UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Datas")
	TSubclassOf<UCActionData> ActionDataClass;*/
};
