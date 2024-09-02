#include "CPlayerAttributeComponent.h"

UCPlayerAttributeComponent::UCPlayerAttributeComponent()
{
	//	PrimaryComponentTick.bCanEverTick = true;

}


void UCPlayerAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UCPlayerAttributeComponent::SetMaxStamina(float value)
{
	MaxStamina = value;
}

void UCPlayerAttributeComponent::SetCurrentStamina(float value)
{
	CurrentStamina = value;
}


//void UCPlayerAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
//{
//	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//
//}

