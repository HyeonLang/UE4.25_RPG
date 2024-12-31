
#include "CInteractionComponent.h"

UCInteractionComponent::UCInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UCInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UCInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

