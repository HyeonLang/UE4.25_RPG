// Fill out your copyright notice in the Description page of Project Settings.


#include "CAction_ResonanceSkill.h"
#include "Character/CPlayerCharacter.h"
#include "Components/CActionComponent.h"
#include "Global.h"
#include "Weapon/CWeapon.h"

#include "Net/UnrealNetwork.h"


UCAction_ResonanceSkill::UCAction_ResonanceSkill()
{
	ActionName = "ResonanceSkill";
}

void UCAction_ResonanceSkill::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);



	if (GetOwningComponent()->GetOwnerRole() == ROLE_Authority)
	{
		if (GetAimTargetDirection(TargetDerection, TargetActor, false))// Todo. bossmode fix
		{
			Instigator->SetActorRotation(TargetDerection);
		}
	}


	ACPlayerCharacter* InstigatorCharacter = Cast<ACPlayerCharacter>(Instigator);
	if (InstigatorCharacter->GetWeapon())
	{
		InstigatorCharacter->GetWeapon()->OnEquip();
	}
}

void UCAction_ResonanceSkill::StopAction_Implementation(AActor* Instigator)
{
	Super::StopAction_Implementation(Instigator);


}

void UCAction_ResonanceSkill::OnRep_Target()
{
}

void UCAction_ResonanceSkill::OnRep_TargetDerection()
{
	GetOwningComponent()->GetOwner()->SetActorRotation(TargetDerection);
}


void UCAction_ResonanceSkill::Attack_Elapsed_Implementation(ACharacter* InstigatorCharacter)
{
}

void UCAction_ResonanceSkill::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCAction_ResonanceSkill, TargetActor);
	DOREPLIFETIME(UCAction_ResonanceSkill, TargetDerection);
}
