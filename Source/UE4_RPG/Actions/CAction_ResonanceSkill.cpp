// Fill out your copyright notice in the Description page of Project Settings.


#include "CAction_ResonanceSkill.h"
#include "Character/CPlayerCharacter.h"
#include "Components/CActionComponent.h"
#include "Global.h"
#include "Weapon/CWeapon.h"

#include "LevelSequence.h"
#include "Net/UnrealNetwork.h"


UCAction_ResonanceSkill::UCAction_ResonanceSkill()
{
	ActionName = "ResonanceSkill";
}

void UCAction_ResonanceSkill::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);


	ACPlayerCharacter* InstigatorCharacter = Cast<ACPlayerCharacter>(Instigator);

	if (GetOwningComponent()->GetOwnerRole() == ROLE_Authority)
	{
		if (GetAimTargetLocationAndDirection(TargetLocation, TargetDerection, TargetActor, false))// Todo. bossmode fix
		{
			Instigator->SetActorRotation(TargetDerection);
			
			if (ActionDatas.IsValidIndex(ComboIndex) && !FMath::IsNearlyZero(ActionDatas[ComboIndex].EffectiveAttackRange))
			{
				float Distance = UKismetMathLibrary::Distance2D(FVector2D(InstigatorCharacter->GetActorLocation().X, InstigatorCharacter->GetActorLocation().Y), FVector2D(TargetLocation.X, TargetLocation.Y));
				
				InstigatorCharacter->MoveToTarget(Distance - ActionDatas[ComboIndex].EffectiveAttackRange);
				
			}
		}
	}


	if (InstigatorCharacter->GetWeapon())
	{
		InstigatorCharacter->GetWeapon()->OnEquip();
	}
}

void UCAction_ResonanceSkill::StopAction_Implementation(AActor* Instigator)
{
	ACPlayerCharacter* InstigatorCharacter = Cast<ACPlayerCharacter>(Instigator);


	Super::StopAction_Implementation(Instigator);
}

void UCAction_ResonanceSkill::OnRep_Target()
{
}

void UCAction_ResonanceSkill::OnRep_TargetLocation()
{
	ACPlayerCharacter* PlayerCharacter = Cast<ACPlayerCharacter>(GetOwningComponent()->GetOwner());
	if (PlayerCharacter)
	{
		if (ActionDatas.IsValidIndex(ComboIndex) && !FMath::IsNearlyZero(ActionDatas[ComboIndex].EffectiveAttackRange))
		{
			float Distance = UKismetMathLibrary::Distance2D(FVector2D(PlayerCharacter->GetActorLocation().X, PlayerCharacter->GetActorLocation().Y), FVector2D(TargetLocation.X, TargetLocation.Y));

			PlayerCharacter->MoveToTarget(Distance - ActionDatas[ComboIndex].EffectiveAttackRange);
		}
	}
}

void UCAction_ResonanceSkill::OnRep_TargetDerection()
{
	GetOwningComponent()->GetOwner()->SetActorRotation(TargetDerection);
}


void UCAction_ResonanceSkill::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCAction_ResonanceSkill, TargetActor);
	DOREPLIFETIME(UCAction_ResonanceSkill, TargetDerection);
	DOREPLIFETIME(UCAction_ResonanceSkill, TargetLocation);
}
