// Fill out your copyright notice in the Description page of Project Settings.


#include "CAbilitySystemComponent.h"

UCAbilitySystemComponent::UCAbilitySystemComponent()
{
	SetIsReplicatedByDefault(true);
	SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
}