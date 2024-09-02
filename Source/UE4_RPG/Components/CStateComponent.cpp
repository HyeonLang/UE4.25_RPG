#include "CStateComponent.h"

UCStateComponent::UCStateComponent()
{
	
}


void UCStateComponent::BeginPlay()
{
	Super::BeginPlay();


	
}


void UCStateComponent::SetNormalMode()
{
}

void UCStateComponent::SetImmunityMode()
{
}

void UCStateComponent::SetUnstoppableMode()
{
}

void UCStateComponent::SetInvulnerabilityMode()
{
}

void UCStateComponent::ChangeStateType(EStateType InNewType)
{
	EStateType Prev = StateType;
	StateType = InNewType;
	
	if (OnStateTypeChanged.IsBound())
	{
		OnStateTypeChanged.Broadcast(Prev, InNewType);
	}
}
