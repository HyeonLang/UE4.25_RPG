#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CPlayerCharacterAnimInstance.generated.h"

class ACPlayerCharacter;
class UCActionComponent;
class UCharacterMovementComponent;
class UCAction;

UCLASS()
class UE4_RPG_API UCPlayerCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UCPlayerCharacterAnimInstance();

protected:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Data")
	ACPlayerCharacter* PlayerCharacter;

	UPROPERTY(BlueprintReadOnly, Category = "Data")
	UCActionComponent* ActionComp;

	UPROPERTY(BlueprintReadOnly, Category = "Data")
	UCharacterMovementComponent* CharacterMovementComp;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Data")
	TSubclassOf<UCAction> JumpClass;

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

	

	//bool bIsAttacking;
	
	float PreSpeed;
	float LeanIntensityScale;

};
