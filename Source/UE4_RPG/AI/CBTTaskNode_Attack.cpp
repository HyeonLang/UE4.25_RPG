#include "CBTTaskNode_Attack.h"

#include "AIController.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Global.h"
#include "Character/CEnemyCharacter.h"
#include "Components/CNPCActionComponent.h"

UCBTTaskNode_Attack::UCBTTaskNode_Attack()
{

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
			//NetMulticastPlayAttack(NPCCharacter, BB);
			return PlayAttack(NPCCharacter, BB);
		}
	}

	return EBTNodeResult::Failed;
}

EBTNodeResult::Type UCBTTaskNode_Attack::PlayAttack_Implementation(ACharacter* InstigatorCharacter, UBlackboardComponent* BlackboardComp)
{
	UCNPCActionComponent* NPCActionComp = Cast<UCNPCActionComponent>(InstigatorCharacter->GetComponentByClass(UCNPCActionComponent::StaticClass()));
	if (NPCActionComp)
	{
		NPCActionComp->StartNPCActionByName(InstigatorCharacter, NPCActionName);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}

void UCBTTaskNode_Attack::NetMulticastPlayAttack_Implementation(ACharacter* InstigatorCharacter, UBlackboardComponent* BlackboardComp)
{
	PlayAttack(InstigatorCharacter, BlackboardComp);
}
