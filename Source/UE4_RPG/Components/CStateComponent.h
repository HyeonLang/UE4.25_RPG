#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CStateComponent.generated.h"

class ACPlayerCharacter;

UENUM(BlueprintType)
enum class EMovementStateType : uint8
{
	Idle, Walking, Running, Sprinting, Jumping, Flying, Max
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FMovementStateTypeChanged, EMovementStateType, InPrevType, EMovementStateType, InNewType);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE4_RPG_API UCStateComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCStateComponent();

protected:
	virtual void BeginPlay() override;

public:
	FORCEINLINE EMovementStateType GetStateType() { return MovementStateType; }

public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE bool IsIdleMode() { return MovementStateType == EMovementStateType::Idle; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE bool IsWalkingMode() { return MovementStateType == EMovementStateType::Walking; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE bool IsRunningMode() { return MovementStateType == EMovementStateType::Running; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE bool IsSprintingMode() { return MovementStateType == EMovementStateType::Sprinting; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE bool IsFlyingMode() { return MovementStateType == EMovementStateType::Jumping; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE bool IsJumpingMode() { return MovementStateType == EMovementStateType::Jumping; }

	UFUNCTION(BlueprintCallable)
	void SetIdleMode();
	UFUNCTION(BlueprintCallable)
	void SetWalkingMode();
	UFUNCTION(BlueprintCallable)
	void SetRunningMode();
	UFUNCTION(BlueprintCallable)
	void SetSprintingMode();
	UFUNCTION(BlueprintCallable)
	void SetJumpingMode();
	UFUNCTION(BlueprintCallable)
	void SetFlyingMode();
	
private:
	void ChangeMovementStateType(EMovementStateType InNewType);

public:
	UPROPERTY(BlueprintAssignable)
	FMovementStateTypeChanged OnMovementStateTypeChanged;
protected:
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "MovementState")
	EMovementStateType MovementStateType;

private:
	ACPlayerCharacter* PlayerCharacter;
		
};
