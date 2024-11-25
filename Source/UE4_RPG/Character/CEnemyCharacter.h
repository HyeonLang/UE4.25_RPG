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
class UCActionComponent;
class UAbilitySystemComponent;
class UCAbilitySystemComponent;
class UCEnemyCharacterAttributeSet;
class UCWorldWidgetComponent;

UCLASS()
class UE4_RPG_API ACEnemyCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ACEnemyCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	//~ IAbilitySystemInterface 시작
	/** 어빌리티 시스템 컴포넌트를 반환합니다. */
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//~ IAbilitySystemInterface 끝

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "State")
	void OnAttackBegin();

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Components")
	FORCEINLINE UCStateComponent* GetStateComponent() const { return StateComp; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Components")
	FORCEINLINE UCActionComponent* GetActionComponent() const { return ActionComp; }

protected:
	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UCAbilitySystemComponent* OwningComp, float NewHealth, float Delta);

protected:
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Component")
	UCStateComponent* StateComp;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Component")
	UCActionComponent* ActionComp;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Component")
	UCAbilitySystemComponent* AbilitySystemComp;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Component")
	UCWorldWidgetComponent* WidgetComp;

	// UI
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> HealthBarWidgetClass;

	UUserWidget* HealthBarWidget;

};
