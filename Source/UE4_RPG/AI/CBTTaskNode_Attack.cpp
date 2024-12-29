#include "CBTTaskNode_Attack.h"

#include "AIController.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Global.h"
#include "Character/CEnemyCharacter.h"
#include "Components/CNPCActionComponent.h"
#include "Actions/CNPCAction.h"

UCBTTaskNode_Attack::UCBTTaskNode_Attack()
{
	bNotifyTick = true;

	bActiveAttack = false;
	ActiveAction = nullptr;
}

EBTNodeResult::Type UCBTTaskNode_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AAIController* AIC = OwnerComp.GetAIOwner();

	if (ensure(AIC))
	{
		ACharacter* NPCCharacter = Cast<ACharacter>(AIC->GetPawn());
		if (!NPCCharacter)
		{
			return EBTNodeResult::Failed;
		}

		UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
		if (ensure(BB))
		{
			return PlayAttack(NPCCharacter, BB);
		}
	}

	return EBTNodeResult::Failed;
}

void UCBTTaskNode_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (ActiveAction)
	{
		if (!ActiveAction->IsRunning() && bActiveAttack == true)
		{
			bActiveAttack = false;
			ActiveAction = nullptr;
			CLog::Print("Succeeded", -1, 2.0f);
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	}
}

EBTNodeResult::Type UCBTTaskNode_Attack::PlayAttack_Implementation(ACharacter* InstigatorCharacter, UBlackboardComponent* BlackboardComp)
{
	UCNPCActionComponent* NPCActionComp = Cast<UCNPCActionComponent>(InstigatorCharacter->GetComponentByClass(UCNPCActionComponent::StaticClass()));
	if (NPCActionComp)
	{
		NPCActionComp->StartNPCActionByName(InstigatorCharacter, NPCActionName);
		ActiveAction = NPCActionComp->GetNPCActionByName(NPCActionName);
		bActiveAttack = true;

		return EBTNodeResult::InProgress;
	}

	return EBTNodeResult::Failed;
}

void UCBTTaskNode_Attack::NetMulticastPlayAttack_Implementation(ACharacter* InstigatorCharacter, UBlackboardComponent* BlackboardComp)
{
	PlayAttack(InstigatorCharacter, BlackboardComp);
}
