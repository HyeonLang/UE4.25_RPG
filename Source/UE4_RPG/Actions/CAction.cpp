#include "CAction.h"

#include "Net/UnrealNetwork.h"

#include "Global.h"
#include "Character/CPlayerCharacter.h"
#include "Components/CActionComponent.h"
#include "Components/CAimingComponent.h"

UCAction::UCAction()
{
	ComboIndex = 0;
	bCanCombo = false;
}


void UCAction::ActionTick_Implementation(float DeltaTime)
{
}

void UCAction::Initialize()
{
	SetActionDatas();

	CurrentComboActionName = ActionName;
}


bool UCAction::CanStart_Implementation(AActor* Instigator, FString& OutMsg)
{
	if (IsRunning())
	{
		OutMsg = "IsRunning";
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
	UE_LOG(LogTemp, Log, TEXT("Started : %s"), *GetNameSafe(this));

	UCActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.AppendTags(GrantTags);
	ACPlayerCharacter* PC = Cast<ACPlayerCharacter>(Instigator);

	// ������ ���
	RepData.bIsRunning = true;
	RepData.Instigator = Instigator;

	if (PC && ActionDatas.IsValidIndex(ComboIndex) && ActionDatas[ComboIndex].MontageDatas.IsValidIndex(0))
	{
		Comp->bCanStopMontagePostAction = ActionDatas[ComboIndex].MontageDatas[0].bCanStopMontagePostAction;
		if (!ActionDatas[ComboIndex].MontageDatas[0].bCanMove)
			PC->CanMoveCount++;
	}

	if (Comp->GetOwnerRole() == ROLE_Authority) // ������� ���������� ����
	{
		TimeStarted = GetWorld()->TimeSeconds; // ���� �ð�
	}

	if (Comp->OnActionStarted.IsBound())
	{
		Comp->OnActionStarted.Broadcast(Comp, this);
		Comp->OnActionStarted.Clear();
	}
}

void UCAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Stopped : %s"), *GetNameSafe(this));

	UCActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.RemoveTags(GrantTags);
	ACPlayerCharacter* PC = Cast<ACPlayerCharacter>(Instigator);

	if (PC)
	{
		if (ActionDatas.IsValidIndex(ComboIndex) && ActionDatas[ComboIndex].MontageDatas.IsValidIndex(0) &&!ActionDatas[ComboIndex].MontageDatas[0].bCanMove)
			PC->CanMoveCount--;
	}

	if (PC->GetAimingComponent()->TargetActor)
	{
		PC->GetAimingComponent()->TargetActor = nullptr;
	}

	IgnoreActors.Empty();

	// ������ ���
	RepData.bIsRunning = false;
	RepData.Instigator = Instigator;

	if (Comp->OnActionStopped.IsBound())
	{
		Comp->OnActionStopped.Broadcast(Comp, this);
		Comp->OnActionStopped.Clear();
	}
}


UWorld* UCAction::GetWorld() const
{
	AActor* Actor = Cast<AActor>(GetOuter());
	if (Actor)
	{
		return Actor->GetWorld();
	}

	return nullptr;
}

UCActionComponent* UCAction::GetOwningComponent() const
{
	return ActionComp;
}

void UCAction::PlayMontageDataAction_Implementation(FActionMontageData MontageData, ACPlayerCharacter* Instigator, bool bBindEndedDelegate)
{
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

void UCAction::PlayMontageAction_Implementation(UAnimMontage* Montage, ACPlayerCharacter* Instigator, bool bBindEndedDelegate)
{
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

void UCAction::GetAimTargetDirection_Implementation(FRotator& OutDirection, AActor* OutTarget, const bool InIsBossMode)
{
	UCAimingComponent* AimingComp = Cast<UCAimingComponent>(GetOwningComponent()->GetOwner()->GetComponentByClass(UCAimingComponent::StaticClass()));
	
	AimingComp->GetAimTargetDirection(OutDirection, OutTarget, ActionDatas[ComboIndex].AttackRange, InIsBossMode);
}

void UCAction::SetCanCombo(bool InNew)
{
	bCanCombo = InNew;
}

void UCAction::SetOwningComponent(UCActionComponent* NewActionComp)
{
	ActionComp = NewActionComp;
	
}

void UCAction::SetActionDatas()
{
	if (ActionDataAssets)
	{
		ActionDataAssets->BeginPlay(this, ActionDatas);
	}
}

void UCAction::BindOnMontageEndedDelegate_Implementation(UAnimMontage* Montage, ACPlayerCharacter* Instigator)
{
	FOnMontageEnded OnMontageEndedDelegate;

	OnMontageEndedDelegate.BindUFunction(this, FName("OnMontageEnded"));
	CLog::Print(OnMontageEndedDelegate.IsBound() ? "true" : "false");
	UAnimInstance* AnimInstance = Instigator->GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->Montage_SetEndDelegate(OnMontageEndedDelegate, Montage);
	}
}

void UCAction::InterruptedAction_Implementation()
{
	UCActionComponent* Comp = GetOwningComponent();
	if (Comp)
	{
		StopAction(Comp->GetOwner());
	}
}

void UCAction::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	CLog::Print("OnMontageEnded");
	UCActionComponent* Comp = GetOwningComponent();
	if (Comp)
	{
		StopAction(Comp->GetOwner());
	}
}

// �ٸ� Ŭ���� �ڱ⸸ ȣ�� : RepData�� ������ �ٸ� ���
void UCAction::OnRep_RepData()
{
	if (RepData.bIsRunning)
	{
		// ���� �ν�Ƽ�����Ͱ� �ƴ� ���� �ν�Ƽ�����͸� �Ѱܾ���
		// �������� StartAction���� �ν�Ƽ�����͸� �־��ذ��� �ٽ� ���ÿ� �Ѱ���
		StartAction(RepData.Instigator);
	}
	else
	{
		StopAction(RepData.Instigator);
	}
}


bool UCAction::IsRunning() const
{
	return RepData.bIsRunning;
}

void UCAction::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCAction, RepData);
	DOREPLIFETIME(UCAction, ActionComp);
	DOREPLIFETIME(UCAction, ActionDatas);
	DOREPLIFETIME(UCAction, TimeStarted);
}