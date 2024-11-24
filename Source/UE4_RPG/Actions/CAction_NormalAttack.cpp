

#include "CAction_NormalAttack.h"
#include "Components/CActionComponent.h"
#include "Components/CAimingComponent.h"
#include "Character/CPlayerCharacter.h"
#include "Weapon/CWeapon.h"


UCAction_NormalAttack::UCAction_NormalAttack()
{
	ActionName = "NormalAttack";

	bUseWeapon = true;
}

void UCAction_NormalAttack::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);
	


	GetAimTargetDirection(TargetDerection, TargetActor, false); // Todo. bossmode fix

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


void UCAction_NormalAttack::Attack_Elapsed_Implementation(ACharacter* InstigatorCharacter)
{
}
