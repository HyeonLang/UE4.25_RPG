#include "CAction_NormalAttack.h"
#include "Components/CActionComponent.h"
#include "Components/CAimingComponent.h"
#include "Character/CPlayerCharacter.h"
#include "Weapon/CWeapon.h"
#include "Global.h"

#include "Net/UnrealNetwork.h"


UCAction_NormalAttack::UCAction_NormalAttack()
{
	ActionName = "NormalAttack";

	bUseWeapon = true;
}

void UCAction_NormalAttack::StartAction_Implementation(AActor* Instigator)
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

void UCAction_NormalAttack::StopAction_Implementation(AActor* Instigator)
{
	ACPlayerCharacter* InstigatorCharacter = Cast<ACPlayerCharacter>(Instigator);
	
	Super::StopAction_Implementation(Instigator);
}

void UCAction_NormalAttack::OnRep_Target()
{
}

void UCAction_NormalAttack::OnRep_TargetDerection()
{
	GetOwningComponent()->GetOwner()->SetActorRotation(TargetDerection);
}

void UCAction_NormalAttack::OnRep_TargetLocation()
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



void UCAction_NormalAttack::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCAction_NormalAttack, TargetActor);
	DOREPLIFETIME(UCAction_NormalAttack, TargetDerection);
	DOREPLIFETIME(UCAction_NormalAttack, TargetLocation);
}