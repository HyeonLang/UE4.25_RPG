#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CStateComponent.generated.h"

UENUM(BlueprintType)
enum class EStateType : uint8
{
	Normal, Immunity, Unstoppable, Invulnerability, Max
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FStateTypeChanged, EStateType, InPrevType, EStateType, InNewType);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE4_RPG_API UCStateComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCStateComponent();

protected:
	virtual void BeginPlay() override;

public:
	FORCEINLINE EStateType GetStateType() { return StateType; }

public:
	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsNormalMode() { return StateType == EStateType::Normal; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsImmunityMode() { return StateType == EStateType::Immunity; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsUnstoppableMode() { return StateType == EStateType::Unstoppable; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsInvulnerabilityMode() { return StateType == EStateType::Invulnerability; }

	void SetNormalMode();
	void SetImmunityMode();
	void SetUnstoppableMode();
	void SetInvulnerabilityMode();
	
private:
	void ChangeStateType(EStateType InNewType);

public:
	UPROPERTY(BlueprintAssignable)
	FStateTypeChanged OnStateTypeChanged;

private:
	EStateType StateType;
		
};
