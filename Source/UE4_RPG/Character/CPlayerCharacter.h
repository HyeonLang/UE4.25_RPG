#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "Interfaces/CIconInterface.h"
#include "CPlayerCharacter.generated.h"

class ACPlayerController;
class UAnimMontage;
class UCameraComponent;
class USpringArmComponent;
class UCAimingComponent;
class UCStateComponent;
class UCActionComponent;
class UCIKComponent;
class UCInteractionComponent;
class UAbilitySystemComponent;
class UCAbilitySystemComponent;
class UCPlayerCharacterAttributeSet;
class ACWeapon;
class UCAction;


UCLASS()
class UE4_RPG_API ACPlayerCharacter : public ACharacter, public IAbilitySystemInterface, public ICIconInterface
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

	virtual TSoftObjectPtr<UTexture2D> GetIcon_Implementation() const override;

	void SetOwnerController(ACPlayerController* Controller);

	
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

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Components")
	FORCEINLINE UCIKComponent* GetIKComponent() const { return IKComp; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Components")
	FORCEINLINE UCInteractionComponent* GetInteractionComponent() const { return InteractionComp; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weapon")
	FORCEINLINE ACWeapon* GetWeapon() const { return Weapon; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Controller")
	FORCEINLINE ACPlayerController* GetOwnerController() const { return OwnerController; }

public:
	FORCEINLINE float GetCooldownCharacterChange() const { return Cooldown_CharacterChange; }
	FORCEINLINE bool GetCanCharacterChange() const { return bCanCharacterChange; }

	FORCEINLINE int32 GetCanMoveCount() const { return CanMoveCount; }

	FORCEINLINE bool GetOnField() const { return bOnField; }

	UFUNCTION(BlueprintCallable, Reliable, Server, Category = "Cooldown")
	void SetCharacterChangeCooldown();

	UFUNCTION(Reliable, Server, Category = "Cooldown")
	void SetCanCharacterChange(bool InNew = true);

	UFUNCTION(Reliable, Server, Category = "Montage")
	void ServerStopAnimMontage(UAnimMontage* AnimMontage = nullptr);

	UFUNCTION(NetMulticast, Reliable, Category = "Montage")
	void NetMulticastStopAnimMontage(UAnimMontage* AnimMontage = nullptr);

	UFUNCTION(BlueprintCallable)
	void SetAllVisibility(bool bNewVisibility);
	
	UFUNCTION(BlueprintCallable)
	void SetOnField(bool InNew);
	UFUNCTION(BlueprintCallable)
	void SetCanJump(bool InNew);
	UFUNCTION(BlueprintCallable)
	void SetCanMove(bool InNew);

protected:
	UFUNCTION(BlueprintNativeEvent)
	void OnActionCreateFinished(UCActionComponent* OwningComp);

public:
	UPROPERTY()
	bool IsActiveMontage;

	UPROPERTY(Replicated)
	bool bCanJump;

	UPROPERTY()
	int32 CanMoveCount;

protected:
	UPROPERTY(BlueprintReadOnly, Replicated, VisibleDefaultsOnly, Category = "Controller")
	ACPlayerController* OwnerController;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Component")
	UCAimingComponent* AimingComp;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Component")
	UCStateComponent* StateComp;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Component")
	UCActionComponent* ActionComp;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Component")
	UCIKComponent* IKComp;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Component")
	UCInteractionComponent* InteractionComp;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Component")
	UCAbilitySystemComponent* AbilitySystemComp;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<ACWeapon> WeaponClass;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	ACWeapon* Weapon;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Weapon")
	FName WeaponSocket;

	//Icon
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "UI")
	TSoftObjectPtr<UTexture2D> Icon;		// soft 래퍼런스

protected:
	UPROPERTY(ReplicatedUsing = "OnRep_OnField")
	bool bOnField;

	UFUNCTION()
	void OnRep_OnField();


protected:
	FTimerHandle TimerHandle_Cooldown_CharacterChange;

	UPROPERTY(Replicated)
	float Cooldown_CharacterChange;

	UPROPERTY(Replicated)
	bool bCanCharacterChange;
};
