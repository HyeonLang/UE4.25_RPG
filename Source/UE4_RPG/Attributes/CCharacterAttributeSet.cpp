// Fill out your copyright notice in the Description page of Project Settings.


#include "CCharacterAttributeSet.h"
#include "Net/UnrealNetwork.h"

UCCharacterAttributeSet::UCCharacterAttributeSet()
{

}

void UCCharacterAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCCharacterAttributeSet, Health, OldHealth);
}

void UCCharacterAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCCharacterAttributeSet, MaxHealth, OldMaxHealth);
}



void UCCharacterAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UCCharacterAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCCharacterAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);

}