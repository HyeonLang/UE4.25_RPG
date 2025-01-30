#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "CEnemyCharacter.generated.h"

class UAnimMontage;
class UCameraComponent;
class UUserWidget;
class USpringArmComponent;
class UCAimingComponent;
class UCStateComponent;
class UCNPCActionComponent;
class UAbilitySystemComponent;
class UCAbilitySystemComponent;
class UPawnSensingComponent;
class UCEnemyCharacterAttributeSet;
class UCWorldWidgetComponent;
class ACWeapon;

UCLASS(Blueprintable)
class UE4_RPG_API ACEnemyCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ACEnemyCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	//~ IAbilitySystemInterface ����
	/** �����Ƽ �ý��� ������Ʈ�� ��ȯ�մϴ�. */
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//~ IAbilitySystemInterface ��

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Components")
	FORCEINLINE UPawnSensingComponent* GetPawnSensingComponent() const { return PawnSensingComp; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Components")
	FORCEINLINE UCStateComponent* GetStateComponent() const { return StateComp; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Components")
	FORCEINLINE UCNPCActionComponent* GetNPCActionComponent() const { return NPCActionComp; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Components")
	FORCEINLINE ACWeapon* GetWeapon() const { return Weapon; }


	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Dead();

	UFUNCTION(BlueprintCallable)
	void SetTargetActor(AActor* NewTarget);

	UFUNCTION(BlueprintCallable)
	AActor* GetTargetActor() const;


protected:
	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UCAbilitySystemComponent* OwningComp, float NewHealth, float Delta);

	UFUNCTION()
	void OnSeePawn(APawn* Pawn);

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastPawnSeen();


public:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	AActor* OwnerActor;

protected:
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Components")
	UPawnSensingComponent* PawnSensingComp;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Component")
	UCStateComponent* StateComp;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Component")
	UCNPCActionComponent* NPCActionComp;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Component")
	UCAbilitySystemComponent* AbilitySystemComp;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Component")
	UCWorldWidgetComponent* WidgetComp;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName TargetActorKeyName;

protected:
	// UI
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> HealthBarWidgetClass;

	UUserWidget* HealthBarWidget;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<ACWeapon> WeaponClass;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Weapon")
	ACWeapon* Weapon;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Weapon")
	FName WeaponSocket;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Weapon")
	bool AlwaysWeaponEquip;



};
