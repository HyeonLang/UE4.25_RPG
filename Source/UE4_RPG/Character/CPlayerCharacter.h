#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "CPlayerCharacter.generated.h"

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

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Component")
	FORCEINLINE UCAimingComponent* GetAimingComponent() const { return AimingComp; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Component")
	FORCEINLINE UCStateComponent* GetStateComponent() const { return StateComp; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Component")
	FORCEINLINE UCActionComponent* GetActionComponent() const { return ActionComp; }

public:
	FORCEINLINE float GetCooldownCharacterChange() const { return Cooldown_CharacterChange; }
	FORCEINLINE bool GetCanCharacterChange() const { return bCanCharacterChange; }

	UFUNCTION(BlueprintCallable, Reliable, Server, Category = "Cooldown")
	void SetCharacterChangeCooldown();

	UFUNCTION(Reliable, Server, Category = "Cooldown")
	void SetCanCharacterChange(bool InNew = true);
	
	
	void SetOnField(bool InNew);

protected:


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
	bool bCanJump;

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
