#include "CNPCAction.h"

#include "Net/UnrealNetwork.h"

#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CNPCActionComponent.h"
#include "Components/CAimingComponent.h"
#include "Game/CCooldownManager.h"
#include "Character/CEnemyCharacter.h"
#include "Weapon/CWeapon.h"

UCNPCAction::UCNPCAction()
{
}

void UCNPCAction::Initialize()
{
	Super::Initialize();
}

bool UCNPCAction::CanStart_Implementation(AActor* Instigator, FString& OutMsg)
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

	UCNPCActionComponent* Comp = GetOwningComponent();

	if (Comp->ActiveGameplayTags.HasAny(BlockedTags))
	{
		OutMsg = "BlockedTags";
		return false;
	}

	return true;
}

void UCNPCAction::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	UE_LOG(LogTemp, Log, TEXT("Started : %s"), *GetNameSafe(this));

	UCNPCActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.AppendTags(GrantTags);
	ACharacter* Character = Cast<ACharacter>(Instigator);

	// 서버일 경우
	RepData.bIsRunning = true;
	RepData.Instigator = Instigator;

	if (Character && ActionDatas.IsValidIndex(ComboIndex) && ActionDatas[ComboIndex].MontageDatas.IsValidIndex(0))
	{
		Comp->bCanStopMontagePostNPCAction = ActionDatas[ComboIndex].MontageDatas[0].bCanStopMontagePostAction;
	}

	if (Comp->GetOwnerRole() == ROLE_Authority) // 서버라면 서버에서만 실행
	{
		TimeStarted = GetWorld()->TimeSeconds; // 시작 시간

		if (ActionDatas.IsValidIndex(ComboIndex)) // 쿨다운 실행
			StartCooldown(ActionDatas[ComboIndex].Cooldown);
	}

	if (Comp->OnNPCActionStarted.IsBound())
	{
		Comp->OnNPCActionStarted.Broadcast(Comp, this);
	}
}

void UCNPCAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Stopped : %s"), *GetNameSafe(this));

	UCNPCActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.RemoveTags(GrantTags);
	ACharacter* Character = Cast<ACharacter>(Instigator);
	ACEnemyCharacter* EnemyCharacter = Cast<ACEnemyCharacter>(Instigator);

	if (EnemyCharacter && EnemyCharacter->GetWeapon())
	{
		EnemyCharacter->GetWeapon()->OffCollision();
	}
	

	IgnoreActors.Empty();

	// 서버일 경우
	RepData.bIsRunning = false;
	RepData.Instigator = Instigator;

	if (Comp->OnNPCActionStopped.IsBound())
	{
		Comp->OnNPCActionStopped.Broadcast(Comp, this);
	}

	Super::StopAction_Implementation(Instigator);
}



void UCNPCAction::SetOwningComponent(UCNPCActionComponent* NewNPCActionComp)
{
	NPCActionComp = NewNPCActionComp;
}

bool UCNPCAction::GetAimTargetDirection_Implementation(FRotator& OutDirection, AActor* OutTarget, const bool InIsBossMode)
{
	bool Result = Super::GetAimTargetDirection_Implementation(OutDirection, OutTarget, InIsBossMode);

	return Result;
}

bool UCNPCAction::SetAimTargetLocation_Implementation(AActor* InTarget, const bool InIsBossMode)
{
	bool Result = Super::SetAimTargetLocation_Implementation(InTarget, InIsBossMode);

	return Result;
}

UCNPCActionComponent* UCNPCAction::GetOwningComponent() const
{
	return NPCActionComp;
}

void UCNPCAction::PlayMontageDataAction_Implementation(FActionMontageData MontageData, ACharacter* Instigator, bool bBindEndedDelegate)
{
	Super::PlayMontageDataAction_Implementation(MontageData, Instigator, bBindEndedDelegate);

	if (!ensure(MontageData.AnimMontage)) return;

	if (GetOwningComponent()->ActiveMontageNPCAction && GetOwningComponent()->ActiveMontageNPCAction->IsRunning() && GetOwningComponent()->ActiveMontageNPCAction != this)
	{
		GetOwningComponent()->ActiveMontageNPCAction->InterruptedAction();
	}

	GetOwningComponent()->ActiveMontageNPCAction = this;

	Instigator->PlayAnimMontage
	(
		MontageData.AnimMontage,
		MontageData.PlayRate,
		MontageData.StartSection
	);
}

void UCNPCAction::PlayMontageAction_Implementation(UAnimMontage* Montage, ACharacter* Instigator, bool bBindEndedDelegate)
{
	Super::PlayMontageAction_Implementation(Montage, Instigator, bBindEndedDelegate);

	if (!ensure(Montage)) return;

	if (GetOwningComponent()->ActiveMontageNPCAction && GetOwningComponent()->ActiveMontageNPCAction->IsRunning() && GetOwningComponent()->ActiveMontageNPCAction != this)
	{
		GetOwningComponent()->ActiveMontageNPCAction->InterruptedAction();
	}

	GetOwningComponent()->ActiveMontageNPCAction = this;

	Instigator->PlayAnimMontage
	(
		Montage
	);
}

void UCNPCAction::InterruptedAction_Implementation()
{
	UCNPCActionComponent* Comp = GetOwningComponent();
	if (Comp)
	{
		StopAction(Comp->GetOwner());
	}

	Super::InterruptedAction_Implementation();
}

void UCNPCAction::BindOnMontageEndDelegate_Implementation(UAnimMontage* Montage, ACharacter* Instigator)
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

void UCNPCAction::OnMontageEnd(UAnimMontage* Montage, bool bInterrupted)
{
	UCNPCActionComponent* Comp = GetOwningComponent();
	if (Comp)
	{
		StopAction(Comp->GetOwner());
	}
}

void UCNPCAction::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCNPCAction, NPCActionComp);
}