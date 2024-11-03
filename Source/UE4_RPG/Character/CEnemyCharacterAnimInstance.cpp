#include "CEnemyCharacterAnimInstance.h"
#include "Global.h"
#include "CEnemyCharacter.h"
#include "Components/CActionComponent.h"
#include "Components/CStateComponent.h"

#include "GameFramework/CharacterMovementComponent.h"

UCEnemyCharacterAnimInstance::UCEnemyCharacterAnimInstance()
{
	FallingRotationRateDiv = 0.15;
	LeanIntensityScale = 7.0f;
}

void UCEnemyCharacterAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	EnemyCharacter = Cast<ACEnemyCharacter>(TryGetPawnOwner());
	if (EnemyCharacter)
	{
		CharacterMovementComp = EnemyCharacter->GetCharacterMovement();
		BaseRotationRate = CharacterMovementComp->RotationRate;
	}
}

void UCEnemyCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!EnemyCharacter) return;

	bIsInAir = CharacterMovementComp->IsFalling() | CharacterMovementComp->IsFlying();

	Speed = EnemyCharacter->GetVelocity().Size2D();
	bIsAccelerating = (Speed - PreSpeed > 0.f) ? true : false;

	FRotator PCAimRot = EnemyCharacter->GetBaseAimRotation();
	FRotator PCRot = EnemyCharacter->GetActorRotation();
	FRotator Rot = UKismetMathLibrary::NormalizedDeltaRotator(PCAimRot, PCRot);
	Roll = Rot.Roll;
	Pitch = Rot.Pitch;
	Yow = Rot.Yaw;

	FRotator LeanRot = UKismetMathLibrary::NormalizedDeltaRotator(RotationLastTick, PCRot);
	float Target = (Rot.Yaw / DeltaSeconds) / LeanIntensityScale;
	YawDelta = UKismetMathLibrary::FInterpTo(YawDelta, Target, DeltaSeconds, 6.f);

	RotationLastTick = EnemyCharacter->GetActorRotation();

	bFullBody = GetCurveValue(TEXT("FullBody")) > 0.0f ? true : false;


	if (bIsInAir)
	{
		EnemyCharacter->GetStateComponent()->SetJumpingMode();
	}
	else
	{

		if (Speed >= 450.f)
		{
			EnemyCharacter->GetStateComponent()->SetSprintingMode();
		}
		else if (Speed >= 150.f)
		{
			EnemyCharacter->GetStateComponent()->SetRunningMode();
		}
		else if (Speed >= 50.f)
		{
			EnemyCharacter->GetStateComponent()->SetWalkingMode();
		}
		else
		{
			EnemyCharacter->GetStateComponent()->SetIdleMode();
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
