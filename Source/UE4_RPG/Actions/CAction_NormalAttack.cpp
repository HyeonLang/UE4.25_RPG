#include "CAction_NormalAttack.h"
#include "Components/CActionComponent.h"
#include "Components/CAimingComponent.h"
#include "Character/CPlayerCharacter.h"
#include "Weapon/CWeapon.h"

#include "Net/UnrealNetwork.h"


UCAction_NormalAttack::UCAction_NormalAttack()
{
	ActionName = "NormalAttack";

	bUseWeapon = true;
}

void UCAction_NormalAttack::StartAction_Implementation(AActor* Instigator)
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

void UCAction_NormalAttack::StopAction_Implementation(AActor* Instigator)
{
	Super::StopAction_Implementation(Instigator);

	
}

void UCAction_NormalAttack::OnRep_Target()
{
}

void UCAction_NormalAttack::OnRep_TargetDerection()
{
	GetOwningComponent()->GetOwner()->SetActorRotation(TargetDerection);
}



void UCAction_NormalAttack::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCAction_NormalAttack, TargetActor);
	DOREPLIFETIME(UCAction_NormalAttack, TargetDerection);
}