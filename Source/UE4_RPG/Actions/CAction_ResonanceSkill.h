// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actions/CAction.h"
#include "CAction_ResonanceSkill.generated.h"


UCLASS()
class UE4_RPG_API UCAction_ResonanceSkill : public UCAction
{
	GENERATED_BODY()
		
public:
	UCAction_ResonanceSkill();

public:
	virtual void StartAction_Implementation(AActor* Instigator) override;
	virtual void StopAction_Implementation(AActor* Instigator) override;

protected:
	UPROPERTY(BlueprintReadWrite, Category = "Target", ReplicatedUsing = "OnRep_Target")
	AActor* TargetActor;

	UFUNCTION()
	void OnRep_Target();

	UPROPERTY(BlueprintReadWrite, Category = "Target", ReplicatedUsing = "OnRep_TargetDerection")
	FRotator TargetDerection;

	UFUNCTION()
	void OnRep_TargetDerection();
};
