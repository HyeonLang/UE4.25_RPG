// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "CCharacterAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class UE4_RPG_API UCCharacterAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UCCharacterAttributeSet();

public:
	ATTRIBUTE_ACCESSORS(UCCharacterAttributeSet, Health);
	ATTRIBUTE_ACCESSORS(UCCharacterAttributeSet, MaxHealth);

	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldHealth);

	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, ReplicatedUsing = "OnRep_Health")
	FGameplayAttributeData Health;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = "OnRep_MaxHealth")
	FGameplayAttributeData MaxHealth;

	
};
