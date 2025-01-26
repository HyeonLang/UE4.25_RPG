// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actions/CAction.h"
#include "CAction_CancelLoop.generated.h"

class UCActionComponent;

UCLASS()
class UE4_RPG_API UCAction_CancelLoop : public UCAction
{
	GENERATED_BODY()
public:
	UCAction_CancelLoop();
public:
	virtual void StartAction_Implementation(AActor* Instigator) override;
	virtual void StopAction_Implementation(AActor* Instigator) override;

	virtual bool CanStart_Implementation(AActor* Instigator, FString& OutMsg) override;

protected:
	UFUNCTION()
	void OnActionStopped(UCActionComponent* ActionComponent, UCAction* Action);


public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FName CancelActionName;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	bool bCancelLoopOfffield;
};
