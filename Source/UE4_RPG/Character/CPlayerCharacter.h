#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "CPlayerCharacter.generated.h"

class UAnimMontage;
class UCameraComponent;
class USpringArmComponent;
class UCAimingComponent;
class UCStateComponent;
class UCActionComponent;
class UAbilitySystemComponent;
class UCAbilitySystemComponent;
class UCPlayerCharacterAttributeSet;


UCLASS()
class UE4_RPG_API ACPlayerCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ACPlayerCharacter();

protected:
	virtual void BeginPlay() override;
	
public:	
	virtual void Tick(float DeltaTime) override;

	//~ IAbilitySystemInterface 시작
	/** 어빌리티 시스템 컴포넌트를 반환합니다. */
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//~ IAbilitySystemInterface 끝

	
public:
	void OnMoveForward(float Axis);
	void OnMoveRight(float Axis);

	void OnTurn(float Axis);
	void OnLookUp(float Axis);

	void OnJump();
	//void OnStopJumping();

	void OnDash();

	void StartSprint();
	void StopSprint();

	void StartNormalAttack();
	void StartResonanceSkill();
	void StartResonanceLiberation();

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Components")
	FORCEINLINE UCAimingComponent* GetAimingComponent() const { return AimingComp; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Components")
	FORCEINLINE UCStateComponent* GetStateComponent() const { return StateComp; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Components")
	FORCEINLINE UCActionComponent* GetActionComponent() const { return ActionComp; }



public:
	FORCEINLINE float GetCooldownCharacterChange() const { return Cooldown_CharacterChange; }
	FORCEINLINE bool GetCanCharacterChange() const { return bCanCharacterChange; }

	FORCEINLINE int32 GetCanMoveCount() const { return CanMoveCount; }

	UFUNCTION(BlueprintCallable, Reliable, Server, Category = "Cooldown")
	void SetCharacterChangeCooldown();

	UFUNCTION(Reliable, Server, Category = "Cooldown")
	void SetCanCharacterChange(bool InNew = true);

	UFUNCTION(Reliable, Server, Category = "Montage")
	void ServerStopAnimMontage(UAnimMontage* AnimMontage = nullptr);

	UFUNCTION(NetMulticast, Reliable, Category = "Montage")
	void NetMulticastStopAnimMontage(UAnimMontage* AnimMontage = nullptr);
	
	UFUNCTION(BlueprintCallable)
	void SetOnField(bool InNew);
	UFUNCTION(BlueprintCallable)
	void SetCanJump(bool InNew);
	UFUNCTION(BlueprintCallable)
	void SetCanMove(bool InNew);

public:
	UPROPERTY()
	bool IsActiveMontage;

	UPROPERTY(Replicated)
	bool bCanJump;

	UPROPERTY()
	int32 CanMoveCount;

protected:
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Component")
	UCAimingComponent* AimingComp;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Component")
	UCStateComponent* StateComp;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Component")
	UCActionComponent* ActionComp;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Component")
	UCAbilitySystemComponent* AbilitySystemComp;

	UPROPERTY(Replicated, VisibleDefaultsOnly, Category = "Attribute")
	const UCPlayerCharacterAttributeSet* AttributeSet;

protected:

	UPROPERTY(ReplicatedUsing = "OnRep_OnField")
	bool bOnField;

	UFUNCTION()
	void OnRep_OnField();
private:
	FTimerHandle TimerHandle_Cooldown_CharacterChange;

	UPROPERTY(Replicated)
	float Cooldown_CharacterChange;

	UPROPERTY(Replicated)
	bool bCanCharacterChange;
};
