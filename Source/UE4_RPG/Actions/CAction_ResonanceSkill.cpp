// Fill out your copyright notice in the Description page of Project Settings.


#include "CAction_ResonanceSkill.h"

UCAction_ResonanceSkill::UCAction_ResonanceSkill()
{
	ActionName = "ResonanceSkill";
}

void UCAction_ResonanceSkill::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	GetAimTargetDirection(TargetDerection, TargetActor, false); // Todo. bossmode fix
}

void UCAction_ResonanceSkill::StopAction_Implementation(AActor* Instigator)
{
	Super::StopAction_Implementation(Instigator);
}

void UCAction_ResonanceSkill::Attack_Elapsed_Implementation(ACharacter* InstigatorCharacter)
{
}
