#include "CAction_ResonanceLiberation.h"
#include "Global.h"
#include "Player/CPlayerCameraActor.h"
#include "GameFramework/SpringArmComponent.h"

#include "Components/CActionComponent.h"


UCAction_ResonanceLiberation::UCAction_ResonanceLiberation()
{
	ActionName = "ResonanceLiberation";

	CameraOffset = FVector();
}

void UCAction_ResonanceLiberation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
}

void UCAction_ResonanceLiberation::StopAction_Implementation(AActor* Instigator)
{
	Super::StopAction_Implementation(Instigator);
}

void UCAction_ResonanceLiberation::Attack_Elapsed_Implementation(ACharacter* InstigatorCharacter)
{
}
