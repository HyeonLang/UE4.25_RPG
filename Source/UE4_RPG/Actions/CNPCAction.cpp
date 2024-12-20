#include "CNPCAction.h"

#include "Net/UnrealNetwork.h"

#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CNPCActionComponent.h"
#include "Components/CAimingComponent.h"
#include "Game/CCooldownManager.h"

UCNPCAction::UCNPCAction()
{
	ComboIndex = 0;
}

void UCNPCAction::Initialize()
{
	SetNPCActionDatas();
	CooldownManager = NewObject<UCCooldownManager>(this);
	CurrentComboNPCActionName = NPCActionName;
}

TSoftObjectPtr<UTexture2D> UCNPCAction::GetIcon_Implementation() const
{
	return Icon;
}

void UCNPCAction::NPCActionTick_Implementation(float DeltaTime)
{
	if (CooldownManager)
	{
		CooldownManager->CooldownTick(DeltaTime);
	}
}

bool UCNPCAction::CanStart_Implementation(AActor* Instigator, FString& OutMsg)
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

	UCNPCActionComponent* Comp = GetOwningComponent();

	if (Comp->ActiveGameplayTags.HasAny(BlockedTags))
	{
		OutMsg = "BlockedTags";
		return false;
	}

	return true;
}

void UCNPCAction::StartNPCAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Started : %s"), *GetNameSafe(this));

	UCNPCActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.AppendTags(GrantTags);
	ACharacter* Character = Cast<ACharacter>(Instigator);

	// ������ ���
	RepData.bIsRunning = true;
	RepData.Instigator = Instigator;

	if (Character && NPCActionDatas.IsValidIndex(ComboIndex) && NPCActionDatas[ComboIndex].MontageDatas.IsValidIndex(0))
	{
		Comp->bCanStopMontagePostNPCAction = NPCActionDatas[ComboIndex].MontageDatas[0].bCanStopMontagePostAction;
	}

	if (Comp->GetOwnerRole() == ROLE_Authority) // ������� ���������� ����
	{
		TimeStarted = GetWorld()->TimeSeconds; // ���� �ð�

		if (NPCActionDatas.IsValidIndex(ComboIndex)) // ��ٿ� ����
			StartCooldown(NPCActionDatas[ComboIndex].Cooldown);
	}

	if (Comp->OnNPCActionStarted.IsBound())
	{
		Comp->OnNPCActionStarted.Broadcast(Comp, this);
	}
}

void UCNPCAction::StopNPCAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Stopped : %s"), *GetNameSafe(this));

	UCNPCActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.RemoveTags(GrantTags);
	ACharacter* Character = Cast<ACharacter>(Instigator);

	IgnoreActors.Empty();

	// ������ ���
	RepData.bIsRunning = false;
	RepData.Instigator = Instigator;

	if (Comp->OnNPCActionStopped.IsBound())
	{
		Comp->OnNPCActionStopped.Broadcast(Comp, this);
	}
}

void UCNPCAction::Attack_Elapsed_Implementation(ACharacter* InstigatorCharacter, int32 AttackIndex)
{
}

void UCNPCAction::Attack_ElapsedByOverlapEvent_Implementation(ACharacter* InstigatorCharacter, AActor* InstigatorActor, const FHitResult& HitResult, int32 AttackIndex)
{
}

void UCNPCAction::StartCooldown_Implementation(float BaseCooldown)
{
	if (FMath::IsNearlyZero(BaseCooldown)) return;

	CooldownManager->StartCooldown(BaseCooldown);
	// �θ� ���� �������� ȣ���Ͽ� ��Ÿ�� ����� ��ٿ� ����
}

bool UCNPCAction::IsRunning() const
{
	return RepData.bIsRunning;
}

void UCNPCAction::SetOwningComponent(UCNPCActionComponent* NewNPCActionComp)
{
	NPCActionComp = NewNPCActionComp;
}

void UCNPCAction::SetNPCActionDatas()
{
	if (NPCActionDataAssets)
	{
		NPCActionDataAssets->BeginPlay(this, NPCActionDatas);
	}
}

UWorld* UCNPCAction::GetWorld() const
{
	AActor* Actor = Cast<AActor>(GetOuter());
	if (Actor)
	{
		return Actor->GetWorld();
	}

	return nullptr;
}

bool UCNPCAction::GetAimTargetDirection_Implementation(FRotator& OutDirection, AActor* OutTarget, const bool InIsBossMode)
{
	return false;
}

bool UCNPCAction::SetAimTargetLocation_Implementation(AActor* InTarget, const bool InIsBossMode)
{
	return false;
}

UCNPCActionComponent* UCNPCAction::GetOwningComponent() const
{
	return NPCActionComp;
}

void UCNPCAction::PlayMontageDataNPCAction_Implementation(FActionMontageData MontageData, ACharacter* Instigator, bool bBindEndedDelegate)
{
	if (!ensure(MontageData.AnimMontage)) return;

	if (GetOwningComponent()->ActiveMontageNPCAction && GetOwningComponent()->ActiveMontageNPCAction->IsRunning() && GetOwningComponent()->ActiveMontageNPCAction != this)
	{
		GetOwningComponent()->ActiveMontageNPCAction->InterruptedNPCAction();
	}

	GetOwningComponent()->ActiveMontageNPCAction = this;

	Instigator->PlayAnimMontage
	(
		MontageData.AnimMontage,
		MontageData.PlayRate,
		MontageData.StartSection
	);
}

void UCNPCAction::PlayMontageNPCAction_Implementation(UAnimMontage* Montage, ACharacter* Instigator, bool bBindEndedDelegate)
{
	if (!ensure(Montage)) return;

	if (GetOwningComponent()->ActiveMontageNPCAction && GetOwningComponent()->ActiveMontageNPCAction->IsRunning() && GetOwningComponent()->ActiveMontageNPCAction != this)
	{
		GetOwningComponent()->ActiveMontageNPCAction->InterruptedNPCAction();
	}

	GetOwningComponent()->ActiveMontageNPCAction = this;

	Instigator->PlayAnimMontage
	(
		Montage
	);
}

void UCNPCAction::InterruptedNPCAction_Implementation()
{
	UCNPCActionComponent* Comp = GetOwningComponent();
	if (Comp)
	{
		StopNPCAction(Comp->GetOwner());
	}
}

void UCNPCAction::BindOnMontageEndDelegate_Implementation(UAnimMontage* Montage, ACharacter* Instigator)
{
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
		StopNPCAction(Comp->GetOwner());
	}
}


// �ٸ� Ŭ���� �ڱ⸸ ȣ�� : RepData�� ������ �ٸ� ���
void UCNPCAction::OnRep_RepData()
{
	if (RepData.bIsRunning)
	{
		// ���� �ν�Ƽ�����Ͱ� �ƴ� ���� �ν�Ƽ�����͸� �Ѱܾ���
		// �������� StartAction���� �ν�Ƽ�����͸� �־��ذ��� �ٽ� ���ÿ� �Ѱ���
		StartNPCAction(RepData.Instigator);
	}
	else
	{
		StopNPCAction(RepData.Instigator);
	}
}

void UCNPCAction::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCNPCAction, RepData);
	DOREPLIFETIME(UCNPCAction, NPCActionComp);
	DOREPLIFETIME(UCNPCAction, NPCActionDatas);
	DOREPLIFETIME(UCNPCAction, TimeStarted);
	DOREPLIFETIME(UCNPCAction, CooldownManager);
}