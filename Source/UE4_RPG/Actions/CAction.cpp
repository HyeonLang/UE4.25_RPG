#include "CAction.h"

#include "Net/UnrealNetwork.h"

#include "Global.h"
#include "Character/CPlayerCharacter.h"
#include "Components/CActionComponent.h"
#include "Components/CAimingComponent.h"
#include "Character/CEnemyCharacter.h"
#include "Game/CCooldownManager.h"
#include "Weapon/CWeapon.h"

UCAction::UCAction()
{
	bCanCombo = false;
}

void UCAction::Initialize()
{
	Super::Initialize();
}

bool UCAction::CanStart_Implementation(AActor* Instigator, FString& OutMsg)
{
	Super::CanStart_Implementation(Instigator, OutMsg);

	if (IsRunning())
	{
		OutMsg = "IsRunning";
		return false;
	}

	if (CooldownManager->IsCooldownActive())
	{
		OutMsg = "Cooldown";
		return false;
	}

	UCActionComponent* Comp = GetOwningComponent();

	if (Comp->ActiveGameplayTags.HasAny(BlockedTags))
	{
		OutMsg = "BlockedTags";
		return false;
	}

	return true;
}

void UCAction::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	UE_LOG(LogTemp, Log, TEXT("Started : %s"), *GetNameSafe(this));

	UCActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.AppendTags(GrantTags);
	ACPlayerCharacter* PC = Cast<ACPlayerCharacter>(Instigator);

	// 서버일 경우
	RepData.bIsRunning = true;
	RepData.Instigator = Instigator;

	if (PC && ActionDatas.IsValidIndex(ComboIndex) && ActionDatas[ComboIndex].MontageDatas.IsValidIndex(0))
	{
		if (!ActionDatas[ComboIndex].MontageDatas[0].bCanMove)
			PC->CanMoveCount++;
	}

	if (Comp->GetOwnerRole() == ROLE_Authority) // 서버라면 서버에서만 실행
	{
		TimeStarted = GetWorld()->TimeSeconds; // 시작 시간

		if (ActionDatas.IsValidIndex(ComboIndex))
			StartCooldown(ActionDatas[ComboIndex].Cooldown);
	}

	if (Comp->OnActionStarted.IsBound())
	{
		Comp->OnActionStarted.Broadcast(Comp, this);
	}
}

void UCAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Stopped : %s"), *GetNameSafe(this));

	UCActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.RemoveTags(GrantTags);
	ACPlayerCharacter* PC = Cast<ACPlayerCharacter>(Instigator);

	if (PC->GetWeapon())
	{
		PC->GetWeapon()->OffCollision();
	}

	if (PC && ActionDatas.IsValidIndex(ComboIndex) && ActionDatas[ComboIndex].MontageDatas.IsValidIndex(0))
	{
		Comp->bCanStopMontagePostAction = ActionDatas[ComboIndex].MontageDatas[0].bCanStopMontagePostAction;
		if (!ActionDatas[ComboIndex].MontageDatas[0].bCanMove)
			PC->CanMoveCount--;
	}
	

	if (PC->GetAimingComponent()->TargetActor)
	{
		PC->GetAimingComponent()->TargetActor = nullptr;
	}

	IgnoreActors.Empty();

	// 서버일 경우
	RepData.bIsRunning = false;
	RepData.Instigator = Instigator;

	if (Comp->OnActionStopped.IsBound())
	{
		Comp->OnActionStopped.Broadcast(Comp, this);
	}

	Super::StopAction_Implementation(Instigator);
}

UCActionComponent* UCAction::GetOwningComponent() const
{
	return ActionComp;
}

void UCAction::PlayMontageDataAction_Implementation(FActionMontageData MontageData, ACharacter* Instigator, bool bBindEndedDelegate)
{
	Super::PlayMontageDataAction_Implementation(MontageData, Instigator, bBindEndedDelegate);

	if (!ensure(MontageData.AnimMontage)) return;

	if (GetOwningComponent()->ActiveMontageAction && GetOwningComponent()->ActiveMontageAction->IsRunning() && GetOwningComponent()->ActiveMontageAction != this)
	{
		GetOwningComponent()->ActiveMontageAction->InterruptedAction();
	}

	GetOwningComponent()->ActiveMontageAction = this;

	Instigator->PlayAnimMontage
	(
		MontageData.AnimMontage,
		MontageData.PlayRate,
		MontageData.StartSection
	);


}

void UCAction::PlayMontageAction_Implementation(UAnimMontage* Montage, ACharacter* Instigator, bool bBindEndedDelegate)
{
	Super::PlayMontageAction_Implementation(Montage, Instigator, bBindEndedDelegate);

	if (!ensure(Montage)) return;

	if (GetOwningComponent()->ActiveMontageAction && GetOwningComponent()->ActiveMontageAction->IsRunning() && GetOwningComponent()->ActiveMontageAction != this)
	{
		GetOwningComponent()->ActiveMontageAction->InterruptedAction();
	}

	GetOwningComponent()->ActiveMontageAction = this;

	Instigator->PlayAnimMontage
	(
		Montage
	);


}

bool UCAction::GetAimTargetDirection_Implementation(FRotator& OutDirection, AActor* OutTarget, const bool InIsBossMode)
{
	bool Result = Super::GetAimTargetDirection_Implementation(OutDirection, OutTarget, InIsBossMode);

	UCAimingComponent* AimingComp = Cast<UCAimingComponent>(GetOwningComponent()->GetOwner()->GetComponentByClass(UCAimingComponent::StaticClass()));
	OutTarget = AimingComp->GetAimTargetDirection(OutDirection, ActionDatas[ComboIndex].AttackRange, InIsBossMode);

	if (OutTarget)
	{
		ACEnemyCharacter* EnemyTarget = Cast<ACEnemyCharacter>(OutTarget);
		if (!EnemyTarget)
		{
			return false;
		}
		return true;
	}
	return false;
}

bool UCAction::SetAimTargetLocation_Implementation(AActor* InTarget, const bool InIsBossMode)
{
	bool Result = Super::SetAimTargetLocation_Implementation(InTarget, InIsBossMode);

	return Result;
}

void UCAction::SetCanCombo(bool InNew)
{
	bCanCombo = InNew;
}


void UCAction::SetOwningComponent(UCActionComponent* NewActionComp)
{
	ActionComp = NewActionComp;
	
}

void UCAction::InterruptedAction_Implementation()
{

	UCActionComponent* Comp = GetOwningComponent();
	if (Comp)
	{
		StopAction(Comp->GetOwner());
	}

	Super::InterruptedAction_Implementation();
}

void UCAction::BindOnMontageEndDelegate_Implementation(UAnimMontage* Montage, ACharacter* Instigator)
{
	Super::BindOnMontageEndDelegate_Implementation(Montage, Instigator);

	FOnMontageEnded OnMontageEndedDelegate;

	OnMontageEndedDelegate.BindUFunction(this, FName("OnMontageEnd"));
	//CLog::Print(OnMontageEndedDelegate.IsBound() ? "true" : "false");
	UAnimInstance* AnimInstance = Instigator->GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->Montage_SetEndDelegate(OnMontageEndedDelegate, Montage);
	}
}

void UCAction::OnMontageEnd(UAnimMontage* Montage, bool bInterrupted)
{
	//CLog::Print("OnMontageEnded");
	UCActionComponent* Comp = GetOwningComponent();
	if (Comp)
	{
		StopAction(Comp->GetOwner());
	}
}


void UCAction::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCAction, ActionComp);
}