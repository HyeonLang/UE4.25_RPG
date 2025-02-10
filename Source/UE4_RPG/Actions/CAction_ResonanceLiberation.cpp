#include "CAction_ResonanceLiberation.h"
#include "Global.h"
#include "Player/CPlayerCineCameraActor.h"
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
	LiberationCameraOutBlendTime = 0.2f;
}
void UCAction_ResonanceLiberation::Initialize()
{
	Super::Initialize();

};

void UCAction_ResonanceLiberation::ActionTick_Implementation(float DeltaTime)
{
	Super::ActionTick_Implementation(DeltaTime);

	//if (IsRunning())
	//{
	//	/*if (LiberationCameraActor)
	//	{
	//		ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwningComponent()->GetOwner());
	//		if (OwnerCharacter)
	//		{
	//			LiberationCameraActor->SetActorLocation(OwnerCharacter->GetMesh()->GetBoneLocation("Bip001") + PlayingCameraOffset);
	//		}
	//	}*/
	//}
	//else
	//{
	//	/*if (LiberationCameraActor)
	//	{
	//		AActor* OwnerActor = GetOwningComponent()->GetOwner();
	//		if (OwnerActor)
	//		{
	//			LiberationCameraActor->SetActorLocation(OwnerActor->GetActorLocation());
	//			LiberationCameraActor->SetActorRotation(FRotator(OwnerActor->GetActorRotation().Pitch, OwnerActor->GetActorRotation().Yaw - 180.f, OwnerActor->GetActorRotation().Roll));
	//		}
	//	}*/
	//}
}

void UCAction_ResonanceLiberation::SetActionDatas_Implementation()
{
	Super::SetActionDatas_Implementation();

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

	if (InstigatorCharacter)
	{
		if (InstigatorCharacter->GetWeapon())
		{
			InstigatorCharacter->GetWeapon()->OnEquip();
		}
	
		StartCameraAction(InstigatorCharacter);
	}
}

void UCAction_ResonanceLiberation::StopAction_Implementation(AActor* Instigator)
{

	ACPlayerCharacter* InstigatorCharacter = Cast<ACPlayerCharacter>(Instigator);
	
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


void UCAction_ResonanceLiberation::StartCameraAction_Implementation(ACPlayerCharacter* InstigatorCharacter)
{
	ACharacter* PlayerCharacter = Cast<ACharacter>(GetOwningComponent()->GetOwner());
	if (!PlayerCharacter) return;

	CLog::Print("insie");
	ACPlayerController* PlayerController = Cast<ACPlayerController>(PlayerCharacter->GetController());
	if (!PlayerController) return;

	if (!PlayerController->IsLocalController()) return;
	if (LiberationCameraActorClass)
	{

		LiberationCameraActor = GetWorld()->SpawnActorDeferred<ACPlayerCineCameraActor>(LiberationCameraActorClass, PlayerCharacter->GetActorTransform(), PlayerCharacter, PlayerCharacter);
		LiberationCameraActor->FinishSpawning(PlayerCharacter->GetActorTransform());

		PlayerController->SetViewTarget(LiberationCameraActor);
		PlayerController->DisableInput(PlayerController);
		LiberationCameraActor->OnPlayCinematicFinished.AddDynamic(this, &UCAction_ResonanceLiberation::OnPlayCinematicFinished);
		PlayCameraAction(InstigatorCharacter);
	}
}

void UCAction_ResonanceLiberation::PlayCameraAction_Implementation(ACPlayerCharacter* InstigatorCharacter)
{
}

void UCAction_ResonanceLiberation::EndCameraAction_Implementation(ACPlayerCharacter* InstigatorCharacter)
{
	
	ACharacter* PlayerCharacter = Cast<ACharacter>(GetOwningComponent()->GetOwner());
	if (!PlayerCharacter) return;
	ACPlayerController* PlayerController = Cast<ACPlayerController>(PlayerCharacter->GetController());
	if (!PlayerController) return;

	if (!PlayerController->IsLocalController()) return;

	PlayerController->SetControlRotation(PlayerCharacter->GetActorRotation());
	PlayerController->PlayerCameraActor->SetActorRotation(PlayerController->GetControlRotation());
	PlayerController->SetViewTargetWithBlend(PlayerController->PlayerCameraActor, LiberationCameraOutBlendTime);
	PlayerController->EnableInput(PlayerController);

	if (LiberationCameraActor && !LiberationCameraActor->IsPendingKill())
	{
		LiberationCameraActor->Destroy();
	}

}

void UCAction_ResonanceLiberation::OnPlayCinematicFinished(ACPlayerCineCameraActor* CineCamera)
{
	ACPlayerCharacter* PlayerCharacter = Cast<ACPlayerCharacter>(CineCamera->GetOwner());
	if (ensure(PlayerCharacter))
	{
		EndCameraAction(PlayerCharacter);
	}

	CineCamera->OnPlayCinematicFinished.RemoveDynamic(this, &UCAction_ResonanceLiberation::OnPlayCinematicFinished);
}




void UCAction_ResonanceLiberation::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCAction_ResonanceLiberation, TargetActor);
	DOREPLIFETIME(UCAction_ResonanceLiberation, TargetDerection);
}