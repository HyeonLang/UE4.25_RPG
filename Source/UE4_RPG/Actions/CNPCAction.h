// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CActionBase.h"
#include "CNPCAction.generated.h"

class UCNPCActionComponent;
class UCActionData;
class UCCooldownManager;
class ACharacter;

UCLASS(Blueprintable)
class UE4_RPG_API UCNPCAction : public UCActionBase
{
	GENERATED_BODY()

public:
	UCNPCAction();

public:
	virtual void Initialize() override;


public:
	virtual void StartAction_Implementation(AActor* Instigator) override;

	virtual void StopAction_Implementation(AActor* Instigator) override;

	virtual bool CanStart_Implementation(AActor* Instigator, FString& OutMsg) override;

public:
	void SetOwningComponent(UCNPCActionComponent* NewNPCActionComp);

public:
	virtual bool GetAimTargetDirection_Implementation(FRotator& OutDirection, AActor* OutTarget, const bool InIsBossMode) override;

	virtual bool SetAimTargetLocation_Implementation(AActor* InTarget, const bool InIsBossMode) override;

	UFUNCTION(BlueprintCallable, Category = "Action")
	UCNPCActionComponent* GetOwningComponent() const;

	virtual void PlayMontageDataAction_Implementation(FActionMontageData MontageData, ACharacter* Instigator, bool bBindEndedDelegate = false) override;

	virtual void PlayMontageAction_Implementation(UAnimMontage* Montage, ACharacter* Instigator, bool bBindEndedDelegate = false) override;

	virtual void InterruptedAction_Implementation() override;

protected:
	virtual void BindOnMontageEndDelegate_Implementation(UAnimMontage* Montage, ACharacter* Instigator) override;

	UFUNCTION()
	void OnMontageEnd(UAnimMontage* Montage, bool bInterrupted);

protected:
	UPROPERTY(Replicated)
	UCNPCActionComponent* NPCActionComp;
};
