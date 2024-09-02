#include "CPlayerCharacterAnimInstance.h"

#include "../Global.h"
#include "CPlayerCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"


void UCPlayerCharacterAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	PlayerCharacter = Cast<ACPlayerCharacter>(TryGetPawnOwner());
	LeanIntensityScale = 7.0f;

}

void UCPlayerCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!PlayerCharacter) return;

	bIsInAir = PlayerCharacter->GetMovementComponent()->IsFalling();
	
	Speed = PlayerCharacter->GetVelocity().Size2D();
	bIsAccelerating = (Speed - PreSpeed > 0.f) ? true : false;


	FRotator PCAimRot = PlayerCharacter->GetBaseAimRotation();
	FRotator PCRot = PlayerCharacter->GetActorRotation();
	FRotator Rot = UKismetMathLibrary::NormalizedDeltaRotator(PCAimRot, PCRot);
	Roll = Rot.Roll;
	Pitch = Rot.Pitch;
	Yow = Rot.Yaw;

	FRotator LeanRot = UKismetMathLibrary::NormalizedDeltaRotator(RotationLastTick, PCRot);
	float Target = (Rot.Yaw / DeltaSeconds) / LeanIntensityScale;
	YawDelta = UKismetMathLibrary::FInterpTo(YawDelta, Target, DeltaSeconds, 6.f);

	RotationLastTick = PlayerCharacter->GetActorRotation();

	bFullBody = GetCurveValue(TEXT("FullBody")) > 0.0f ? true : false;
}
