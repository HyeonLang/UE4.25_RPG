#include "CAction_ResonanceLiberation.h"
#include "Global.h"
#include "Player/CPlayerCameraActor.h"
#include "Player/CPlayerController.h"
#include "Character/CPlayerCharacter.h"
#include "Components/CActionComponent.h"


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

	GetAimTargetDirection(TargetDerection, TargetActor, false); // Todo. bossmode fix



	ACPlayerCharacter* InstigatorCharacter = Cast<ACPlayerCharacter>(Instigator);
	if (InstigatorCharacter)
	{
		PlayCameraAction_Implementation(InstigatorCharacter);
	}
}

void UCAction_ResonanceLiberation::StopAction_Implementation(AActor* Instigator)
{
	Super::StopAction_Implementation(Instigator);

	
}

void UCAction_ResonanceLiberation::SetPlayingCameraOffset(FVector NewOffset)
{
	PlayingCameraOffset = NewOffset;
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

void UCAction_ResonanceLiberation::Attack_Elapsed_Implementation(ACharacter* InstigatorCharacter)
{
}


void UCAction_ResonanceLiberation::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCAction_ResonanceLiberation, LiberationCameraActor);
}