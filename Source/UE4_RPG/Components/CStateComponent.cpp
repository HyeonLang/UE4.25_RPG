#include "CStateComponent.h"

#include "Character/CPlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

UCStateComponent::UCStateComponent()
{
	SetIsReplicatedByDefault(true);
	MovementStateType = EMovementStateType::Idle;

	MaxSpeeds.Add(0.f);
	MaxSpeeds.Add(100.f);
	MaxSpeeds.Add(500.f);
	MaxSpeeds.Add(850.f);
	MaxSpeeds.Add(500.f);
	MaxSpeeds.Add(500.f);
}


void UCStateComponent::BeginPlay()
{
	Super::BeginPlay();

	PlayerCharacter = Cast<ACPlayerCharacter>(GetOwner());
	
}


void UCStateComponent::SetIdleMode()
{
	ChangeMovementStateType(EMovementStateType::Idle);
}

void UCStateComponent::SetWalkingMode()
{
	ChangeMovementStateType(EMovementStateType::Walking);
}

void UCStateComponent::SetRunningMode()
{
	ChangeMovementStateType(EMovementStateType::Running);
}

void UCStateComponent::SetSprintingMode()
{
	ChangeMovementStateType(EMovementStateType::Sprinting);
}

void UCStateComponent::SetJumpingMode()
{
	ChangeMovementStateType(EMovementStateType::Jumping);
}

void UCStateComponent::SetFlyingMode()
{
	ChangeMovementStateType(EMovementStateType::Flying);
}

void UCStateComponent::ChangeMovementStateType(EMovementStateType InNewType)
{
	EMovementStateType Prev = MovementStateType;
	MovementStateType = InNewType;

	if (OnMovementStateTypeChanged.IsBound())
	{
		OnMovementStateTypeChanged.Broadcast(Prev, InNewType);
	}
}

//void UCStateComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
//{
//	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
//
//	//DOREPLIFETIME(UCStateComponent, MovementStateType);
//}

