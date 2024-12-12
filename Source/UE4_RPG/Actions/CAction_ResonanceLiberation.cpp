#include "CAction_ResonanceLiberation.h"
#include "Global.h"
#include "Player/CPlayerCameraActor.h"
#include "Player/CPlayerController.h"
#include "Character/CPlayerCharacter.h"
#include "Components/CActionComponent.h"
#include "Weapon/CWeapon.h"

#include "Net/UnrealNetwork.h"
#include "GameFramework/SpringArmComponent.h"

UCAction_ResonanceLiberation::UCAction_ResonanceLiberation()
{
	ActionName = "ResonanceLiberation";

	PlayingCameraOffset = FVector();
}

void UCAction_ResonanceLiberation::ActionTick_Implementation(float DeltaTime)
{
	Super::ActionTick_Implementation(DeltaTime);

	if (IsRunning())
	{
		if (LiberationCameraActor)
		{
			ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwningComponent()->GetOwner());
			if (OwnerCharacter)
			{
				LiberationCameraActor->SetActorLocation(OwnerCharacter->GetMesh()->GetBoneLocation("Bip001") + PlayingCameraOffset);
			}
		}
	}
	else
	{
		if (LiberationCameraActor)
		{
			AActor* OwnerActor = GetOwningComponent()->GetOwner();
			if (OwnerActor)
			{
				LiberationCameraActor->SetActorLocation(OwnerActor->GetActorLocation());
				LiberationCameraActor->SetActorRotation(FRotator(OwnerActor->GetActorRotation().Pitch, OwnerActor->GetActorRotation().Yaw - 180.f, OwnerActor->GetActorRotation().Roll));
			}
		}
	}
}

void UCAction_ResonanceLiberation::SetActionDatas()
{
	Super::SetActionDatas();

	APawn* InstigatorPawn = Cast<APawn>(GetOwningComponent()->GetOwner());
	LiberationCameraActor = GetWorld()->SpawnActorDeferred<ACPlayerCameraActor>(ACPlayerCameraActor::StaticClass(), FTransform(), InstigatorPawn, InstigatorPawn);

	FTransform TM;
	TM.SetLocation(InstigatorPawn->GetActorLocation());
	TM.SetRotation(FRotator(InstigatorPawn->GetActorRotation().Pitch, InstigatorPawn->GetActorRotation().Yaw - 180.f, InstigatorPawn->GetActorRotation().Roll).Quaternion());
	LiberationCameraActor->FinishSpawning(TM);
	LiberationCameraActor->GetSpringArmComp()->TargetArmLength = 200.f;

}

void UCAction_ResonanceLiberation::StartAction_Implementation(AActor* Instigator)
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

	if (InstigatorCharacter && InstigatorCharacter->GetWeapon())
	{
		InstigatorCharacter->GetWeapon()->OnEquip();
	

		LiberationCameraActor->SetActorRotation(FRotator(InstigatorCharacter->GetActorRotation().Pitch, InstigatorCharacter->GetActorRotation().Yaw - 180.f, InstigatorCharacter->GetActorRotation().Roll));
		PlayCameraAction_Implementation(InstigatorCharacter);
	}
}

void UCAction_ResonanceLiberation::StopAction_Implementation(AActor* Instigator)
{

	ACPlayerCharacter* InstigatorCharacter = Cast<ACPlayerCharacter>(Instigator);
	if (InstigatorCharacter->GetWeapon())
	{
		InstigatorCharacter->GetWeapon()->OffCollision();
	}
	
	Super::StopAction_Implementation(Instigator);
}

void UCAction_ResonanceLiberation::SetPlayingCameraOffset(FVector NewOffset)
{
	PlayingCameraOffset = NewOffset;
}

void UCAction_ResonanceLiberation::OnRep_Target()
{
}

void UCAction_ResonanceLiberation::OnRep_TargetDerection()
{
	GetOwningComponent()->GetOwner()->SetActorRotation(TargetDerection);
}


void UCAction_ResonanceLiberation::PlayCameraAction_Implementation(ACPlayerCharacter* InstigatorCharacter)
{
	ACPlayerController* PlayerController = Cast<ACPlayerController>(InstigatorCharacter->GetController());
	if (!PlayerController) return;
	if (!PlayerController->IsLocalController()) return;

	PlayerController->SetViewTarget(LiberationCameraActor);
}

void UCAction_ResonanceLiberation::EndCameraAction_Implementation(ACPlayerCharacter* InstigatorCharacter)
{
	ACPlayerController* PlayerController = Cast<ACPlayerController>(InstigatorCharacter->GetController());
	if (!PlayerController) return;
	if (!PlayerController->IsLocalController()) return;
	PlayerController->SetControlRotation(InstigatorCharacter->GetActorRotation());
	PlayerController->PlayerCameraActor->SetActorRotation(PlayerController->GetControlRotation());
	PlayerController->SetViewTarget(PlayerController->PlayerCameraActor);
}



void UCAction_ResonanceLiberation::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCAction_ResonanceLiberation, LiberationCameraActor);
	DOREPLIFETIME(UCAction_ResonanceLiberation, TargetActor);
	DOREPLIFETIME(UCAction_ResonanceLiberation, TargetDerection);
}