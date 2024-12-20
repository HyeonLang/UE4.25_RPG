#pragma once

#include "CoreMinimal.h"
#include "CActionBase.h"
#include "CAction.generated.h"

class UCActionComponent;
class UCActionData;
class UCCooldownManager;
class ACharacter;


UCLASS(Blueprintable)
class UE4_RPG_API UCAction : public UCActionBase
{
	GENERATED_BODY()

public:
	UCAction();
	
public:
	virtual void Initialize() override;

public:
	virtual void StartAction_Implementation(AActor* Instigator) override;

	virtual void StopAction_Implementation(AActor* Instigator) override;

	virtual bool CanStart_Implementation(AActor* Instigator, FString& OutMsg) override;


public:
	UFUNCTION(BlueprintCallable, Category = "Combo")
	void SetCanCombo(bool InNew);

	void SetOwningComponent(UCActionComponent* NewActionComp);

public:
	virtual bool GetAimTargetDirection_Implementation(FRotator& OutDirection, AActor* OutTarget, const bool InIsBossMode) override;

	virtual bool SetAimTargetLocation_Implementation(AActor* InTarget, const bool InIsBossMode) override;

	UFUNCTION(BlueprintCallable, Category = "Action")
	UCActionComponent* GetOwningComponent() const;

	UFUNCTION(BlueprintCallable, Category = "Combo")
	FORCEINLINE bool GetCanCombo() const { return bCanCombo; }

	virtual void PlayMontageDataAction_Implementation(FActionMontageData MontageData, ACharacter* Instigator, bool bBindEndedDelegate = false) override;

	virtual void PlayMontageAction_Implementation(UAnimMontage* Montage, ACharacter* Instigator, bool bBindEndedDelegate = false) override;

	virtual void InterruptedAction_Implementation() override;


protected:
	virtual void BindOnMontageEndDelegate_Implementation(UAnimMontage* Montage, ACharacter* Instigator) override;

	UFUNCTION()
	void OnMontageEnd(UAnimMontage* Montage, bool bInterrupted);


protected:
	UPROPERTY(Replicated)
	UCActionComponent* ActionComp;

protected:
	bool bCanCombo;



	/*UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Datas")
	TSubclassOf<UCActionData> ActionDataClass;*/
};
