
#include "CActionBase.h"

#include "Net/UnrealNetwork.h"

#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CAimingComponent.h"
#include "Game/CCooldownManager.h"

UCActionBase::UCActionBase()
{
	ComboIndex = 0;
}

void UCActionBase::Initialize()
{
	SetActionDatas();
	CooldownManager = NewObject<UCCooldownManager>(this);
	CurrentComboActionName = ActionName;
}

TSoftObjectPtr<UTexture2D> UCActionBase::GetIcon_Implementation() const
{
	return Icon;
}

void UCActionBase::ActionTick_Implementation(float DeltaTime)
{
	if (CooldownManager)
	{
		CooldownManager->CooldownTick(DeltaTime);
	}
}

void UCActionBase::StartAction_Implementation(AActor* Instigator)
{
	bCanCancelLoop = false;
}

void UCActionBase::StopAction_Implementation(AActor* Instigator)
{
}

void UCActionBase::Attack_Elapsed_Implementation(ACharacter* InstigatorCharacter, int32 AttackIndex)
{
}

void UCActionBase::Attack_ElapsedByOverlapEvent_Implementation(ACharacter* InstigatorCharacter, AActor* InstigatorActor, const FHitResult& HitResult, int32 AttackIndex)
{
}

bool UCActionBase::CanStart_Implementation(AActor* Instigator, FString& OutMsg)
{
	return false;
}

void UCActionBase::StartCooldown_Implementation(float BaseCooldown)
{
	if (FMath::IsNearlyZero(BaseCooldown)) return;

	CooldownManager->StartCooldown(BaseCooldown);
	// �θ� ���� �������� ȣ���Ͽ� ��Ÿ�� ����� ��ٿ� ����
}

bool UCActionBase::IsRunning() const
{
	return RepData.bIsRunning;
}



void UCActionBase::SetActionDatas_Implementation()
{
	if (ActionDataAssets)
	{
		ActionDataAssets->BeginPlay(this, ActionDatas);
	}
}

UWorld* UCActionBase::GetWorld() const
{
	AActor* Actor = Cast<AActor>(GetOuter());
	if (Actor)
	{
		return Actor->GetWorld();
	}

	return nullptr;
}

void UCActionBase::BindOnMontageEndDelegate_Implementation(UAnimMontage* Montage, ACharacter* Instigator)
{
}

bool UCActionBase::GetAimTargetDirection_Implementation(FRotator& OutDirection, AActor* OutTarget, const bool InIsBossMode)
{
	return false;
}

bool UCActionBase::SetAimTargetLocation_Implementation(AActor* InTarget, const bool InIsBossMode)
{
	return false;
}

void UCActionBase::PlayMontageDataAction_Implementation(FActionMontageData MontageData, ACharacter* Instigator, bool bBindEndedDelegate)
{
}

void UCActionBase::PlayMontageAction_Implementation(UAnimMontage* Montage, ACharacter* Instigator, bool bBindEndedDelegate)
{
}

void UCActionBase::InterruptedAction_Implementation()
{
}


void UCActionBase::OnRep_RepData()
{
	if (RepData.bIsRunning)
	{
		// ���� �ν�Ƽ�����Ͱ� �ƴ� ���� �ν�Ƽ�����͸� �Ѱܾ���
		// �������� StartAction���� �ν�Ƽ�����͸� �־��ذ��� �ٽ� ���ÿ� �Ѱ���
		StartAction(RepData.Instigator);
		CLog::Print("In", -1, 2.0f);
	}
	else
	{
		StopAction(RepData.Instigator);
	}
}

void UCActionBase::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCActionBase, RepData);
	DOREPLIFETIME(UCActionBase, ActionDatas);
	DOREPLIFETIME(UCActionBase, TimeStarted);
	DOREPLIFETIME(UCActionBase, CooldownManager);
}