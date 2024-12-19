#include "CAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"


void ACAIController::BeginPlay()
{
	Super::BeginPlay();

	if (ensureMsgf(BehaviorTree, TEXT("BTAsset is not set. Please assign BTAsset in AIController")))
	{
		RunBehaviorTree(BehaviorTree);
	}
}