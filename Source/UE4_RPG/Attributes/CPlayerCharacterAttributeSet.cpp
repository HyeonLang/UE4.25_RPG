// Fill out your copyright notice in the Description page of Project Settings.


#include "CPlayerCharacterAttributeSet.h"
#include "Net/UnrealNetwork.h"


UCPlayerCharacterAttributeSet::UCPlayerCharacterAttributeSet()
{

}


void UCPlayerCharacterAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCPlayerCharacterAttributeSet, Health, OldHealth);
}

void UCPlayerCharacterAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCPlayerCharacterAttributeSet, MaxHealth, OldMaxHealth);
}



void UCPlayerCharacterAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UCPlayerCharacterAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCPlayerCharacterAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);

}
