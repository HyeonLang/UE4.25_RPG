
#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode_Attack.generated.h"

class UCActionBase;

UCLASS()
class UE4_RPG_API UCBTTaskNode_Attack : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UCBTTaskNode_Attack();
	
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

protected:
	UFUNCTION(BlueprintNativeEvent)
	EBTNodeResult::Type PlayAttack(ACharacter* InstigatorCharacter, UBlackboardComponent* BlackboardComp);

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastPlayAttack(ACharacter* InstigatorCharacter, UBlackboardComponent* BlackboardComp);

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AI")
	FName NPCActionName;

protected:
	bool bActiveAttack;

	UCActionBase* ActiveAction;
};
