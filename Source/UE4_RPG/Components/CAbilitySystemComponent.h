// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "CAbilitySystemComponent.generated.h"

class UCCharacterAttributeSet;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnAttributeChanged, AActor*, InstigatorActor, class UCAbilitySystemComponent*, OwningComp, float, NewValue, float, Delta);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeSetFinished,  UCAbilitySystemComponent*, AbilitySystemComponent);
 
UCLASS()
class UE4_RPG_API UCAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:
	UCAbilitySystemComponent();

public:
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	static UCAbilitySystemComponent* GetAbilitySystemComponent(AActor* FromActor);

protected:
	template <class T>
	FORCEINLINE T* GetSetNonConst()
	{
		return const_cast<T*>(GetSet<T>());
	}

public:
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool ApplyHealthChange(AActor* InstigatorActor, float Delta);

	// ApplyHealthChange 넷멀티캐스트
	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastHealthChanged(AActor* InstigatorActor, float NewHealth, float Delta);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool IsAlive() const;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool IsFullHealth() const;

	//UFUNCTION(BlueprintCallable, Category = "Cheat")
	//	bool Kill(AActor* InstigatorActor);

public:
	FOnAttributeSetFinished OnAttributeSetFinished;

public:
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChanged OnHealthChanged;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Attribute")
	UCCharacterAttributeSet* AttributeSet;
};
