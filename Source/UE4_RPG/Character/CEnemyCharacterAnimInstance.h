// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CEnemyCharacterAnimInstance.generated.h"

class ACEnemyCharacter;
class UCharacterMovementComponent;


UCLASS()
class UE4_RPG_API UCEnemyCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UCEnemyCharacterAnimInstance();

protected:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Data")
	ACEnemyCharacter* EnemyCharacter;

	UPROPERTY(BlueprintReadOnly, Category = "Data")
	UCharacterMovementComponent* CharacterMovementComp;

	UPROPERTY(BlueprintReadOnly, Category = "Data")
	float Speed;

	UPROPERTY(BlueprintReadOnly, Category = "Data")
	float Pitch;

	UPROPERTY(BlueprintReadOnly, Category = "Data")
	float Roll;

	UPROPERTY(BlueprintReadOnly, Category = "Data")
	float Yow;

	UPROPERTY(BlueprintReadOnly, Category = "Data")
	float YawDelta;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Data")
	float FallingRotationRateDiv;

	UPROPERTY(BlueprintReadOnly, Category = "Data")
	FRotator RotationLastTick;

	UPROPERTY(BlueprintReadOnly, Category = "Data")
	bool bIsInAir;

	UPROPERTY(BlueprintReadOnly, Category = "Data")
	bool bIsAccelerating;

	UPROPERTY(BlueprintReadOnly, Category = "Data")
	bool bFullBody;

	UPROPERTY(BlueprintReadOnly, Category = "Data")
	FRotator BaseRotationRate;


	float PreSpeed;
	float LeanIntensityScale;
	
};
