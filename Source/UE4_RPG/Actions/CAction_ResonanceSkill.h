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

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Attack_Elapsed(ACharacter* InstigatorCharacter);

protected:
	UPROPERTY(BlueprintReadWrite, Category = "Target")
	AActor* TargetActor;
	UPROPERTY(BlueprintReadWrite, Category = "Target")
	FRotator TargetDerection;


};
