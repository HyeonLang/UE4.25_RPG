#include "CPlayerCharacterAnimInstance.h"

#include "../Global.h"
#include "CPlayerCharacter.h"
#include "Actions/CAction.h"
#include "Components/CActionComponent.h"
#include "Components/CStateComponent.h"

#include "GameFramework/CharacterMovementComponent.h"

UCPlayerCharacterAnimInstance::UCPlayerCharacterAnimInstance()
{
	FallingRotationRateDiv = 0.15;
	LeanIntensityScale = 7.0f;
}

void UCPlayerCharacterAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	PlayerCharacter = Cast<ACPlayerCharacter>(TryGetPawnOwner());
	if (PlayerCharacter)
	{
		ActionComp = PlayerCharacter->GetActionComponent();
		CharacterMovementComp = PlayerCharacter->GetCharacterMovement();
		BaseRotationRate = CharacterMovementComp->RotationRate;
	}

}

void UCPlayerCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!PlayerCharacter) return;

	bIsInAir = CharacterMovementComp->IsFalling() | CharacterMovementComp->IsFlying();
	
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

	PlayerCharacter->IsActiveMontage = IsAnyMontagePlaying();
	
	
	
	if (bIsInAir)
	{
		PlayerCharacter->GetStateComponent()->SetJumpingMode();
	}
	else
	{

		if (Speed >= 450.f)
		{
			PlayerCharacter->GetStateComponent()->SetSprintingMode();
		}
		else if (Speed >= 150.f)
		{
			PlayerCharacter->GetStateComponent()->SetRunningMode();
		}
		else if (Speed >= 50.f)
		{
			PlayerCharacter->GetStateComponent()->SetWalkingMode();
		}
		else
		{
			PlayerCharacter->GetStateComponent()->SetIdleMode();
		}
	}


	if (ActionComp)
	{
		if (JumpClass)
		{
			if (ActionComp->GetAction(JumpClass)->IsRunning() && CharacterMovementComp->IsWalking())
			{
				ActionComp->StopActionByName(PlayerCharacter, TEXT("Jump"));
			}
		}
	}

	if (CharacterMovementComp)
	{
		if (CharacterMovementComp->IsFalling())
		{
			CharacterMovementComp->RotationRate = BaseRotationRate * FallingRotationRateDiv;
		}
		else
		{
			CharacterMovementComp->RotationRate = BaseRotationRate;
		}
	}
}
