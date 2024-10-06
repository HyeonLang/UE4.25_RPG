#include "CAction.h"

#include "Net/UnrealNetwork.h"

#include "Global.h"
#include "Character/CPlayerCharacter.h"
#include "Components/CActionComponent.h"
#include "Components/CAimingComponent.h"

UCAction::UCAction()
{
	ComboIndex = 0;
}

bool UCAction::CanStart_Implementation(AActor* Instigator)
{
	if (IsRunning())
	{
		return false;
	}

	UCActionComponent* Comp = GetOwningComponent();

	if (Comp->ActiveGameplayTags.HasAny(BlockedTags))
	{
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
	if (PC)
	{
		PC->bCanMove = ActionDatas[ComboIndex].MontageDatas[0].bCanMove;
	}

	// ������ ���
	RepData.bIsRunning = true;
	RepData.Instigator = Instigator;

	if (Comp->GetOwnerRole() == ROLE_Authority) // ������� ���������� ����
	{
		TimeStarted = GetWorld()->TimeSeconds; // ���� �ð�
	}

	Comp->OnActionStarted.Broadcast(Comp, this);
}

void UCAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Stopped : %s"), *GetNameSafe(this));

	UCActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.RemoveTags(GrantTags);

	ACPlayerCharacter* PC = Cast<ACPlayerCharacter>(Instigator);
	if (PC)
	{
		PC->bCanMove = true;
	}

	// ������ ���
	RepData.bIsRunning = false;
	RepData.Instigator = Instigator;

	Comp->OnActionStopped.Broadcast(Comp, this);
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

void UCAction::GetAimTargetDirection_Implementation(FRotator& OutDirection, AActor* OutTarget, const bool InIsBossMode)
{
	UCAimingComponent* AimingComp = Cast<UCAimingComponent>(GetOwningComponent()->GetOwner()->GetComponentByClass(UCAimingComponent::StaticClass()));

	AimingComp->GetAimTargetDirection(OutDirection, OutTarget, ActionDatas[ComboIndex].AttackRange, InIsBossMode);
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