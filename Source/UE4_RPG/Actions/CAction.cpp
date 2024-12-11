#include "CAction.h"

#include "Net/UnrealNetwork.h"

#include "Global.h"
#include "Character/CPlayerCharacter.h"
#include "Components/CActionComponent.h"
#include "Components/CAimingComponent.h"
#include "Character/CEnemyCharacter.h"
#include "Game/CCooldownManager.h"

UCAction::UCAction()
{
	ComboIndex = 0;
	bCanCombo = false;
}


void UCAction::ActionTick_Implementation(float DeltaTime)
{
	if (CooldownManager)
	{
		CooldownManager->CooldownTick(DeltaTime);
		
	/*	if (ActionDatas.Num() > 0 && ActionDatas[0].Cooldown > 0)
			CLog::Print(CooldownManager->GetRemainingCooldown(), -1, DeltaTime, FColor::Red);*/
	}
}

void UCAction::Initialize()
{
	SetActionDatas();
	CooldownManager = NewObject<UCCooldownManager>(this);
	CurrentComboActionName = ActionName;
}

TSoftObjectPtr<UTexture2D> UCAction::GetIcon_Implementation() const
{
	return Icon;
}


bool UCAction::CanStart_Implementation(AActor* Instigator, FString& OutMsg)
{
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
	UE_LOG(LogTemp, Log, TEXT("Started : %s"), *GetNameSafe(this));

	UCActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.AppendTags(GrantTags);
	ACPlayerCharacter* PC = Cast<ACPlayerCharacter>(Instigator);

	// 서버일 경우
	RepData.bIsRunning = true;
	RepData.Instigator = Instigator;

	if (PC && ActionDatas.IsValidIndex(ComboIndex) && ActionDatas[ComboIndex].MontageDatas.IsValidIndex(0))
	{
		Comp->bCanStopMontagePostAction = ActionDatas[ComboIndex].MontageDatas[0].bCanStopMontagePostAction;
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

	// 서버일 경우
	RepData.bIsRunning = false;
	RepData.Instigator = Instigator;

	if (Comp->OnActionStopped.IsBound())
	{
		Comp->OnActionStopped.Broadcast(Comp, this);
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

bool UCAction::GetAimTargetDirection_Implementation(FRotator& OutDirection, AActor* OutTarget, const bool InIsBossMode)
{
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
	return false;
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

// 다른 클라의 자기만 호출 : RepData가 서버와 다른 경우
void UCAction::OnRep_RepData()
{
	if (RepData.bIsRunning)
	{
		// 로컬 인스티게이터가 아닌 서버 인스티게이터를 넘겨야함
		// 서버에서 StartAction내의 인스티게이터를 넣어준것을 다시 로컬에 넘겨줌
		StartAction(RepData.Instigator);
	}
	else
	{
		StopAction(RepData.Instigator);
	}
}


void UCAction::Attack_Elapsed_Implementation(ACharacter* InstigatorCharacter, int32 AttackIndex)
{
}

bool UCAction::IsRunning() const
{
	return RepData.bIsRunning;
}

void UCAction::StartCooldown_Implementation(float BaseCooldown)
{
	if (FMath::IsNearlyZero(BaseCooldown)) return;

	CooldownManager->StartCooldown(BaseCooldown);
	// 부모를 제일 마지막에 호출하여 쿨타임 계산후 쿨다운 실행
}

void UCAction::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCAction, RepData);
	DOREPLIFETIME(UCAction, ActionComp);
	DOREPLIFETIME(UCAction, ActionDatas);
	DOREPLIFETIME(UCAction, TimeStarted);
	DOREPLIFETIME(UCAction, CooldownManager);
}