// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "CEnemyCharacterAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class UE4_RPG_API UCEnemyCharacterAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UCEnemyCharacterAttributeSet();

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, ReplicatedUsing = "OnRep_Health")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UCEnemyCharacterAttributeSet, Health);
	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldHealth);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = "OnRep_MaxHealth")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UCEnemyCharacterAttributeSet, MaxHealth);
	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);
	
};
