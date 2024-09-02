#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CPlayerCharacterAnimInstance.generated.h"

class ACPlayerCharacter;

UCLASS()
class UE4_RPG_API UCPlayerCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
protected:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	ACPlayerCharacter* PlayerCharacter;

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

	UPROPERTY(BlueprintReadOnly, Category = "Data")
	FRotator RotationLastTick;

	UPROPERTY(BlueprintReadOnly, Category = "Data")
	bool bIsInAir;

	UPROPERTY(BlueprintReadOnly, Category = "Data")
	bool bIsAccelerating;

	UPROPERTY(BlueprintReadOnly, Category = "Data")
	bool bFullBody;

	//bool bIsAttacking;
	
	float PreSpeed;
	float LeanIntensityScale;

};
