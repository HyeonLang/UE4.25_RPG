
#include "CAction_CancelLoop.h"
#include "Components/CActionComponent.h"



UCAction_CancelLoop::UCAction_CancelLoop()
{
	bCancelLoopOfffield = true;
}

void UCAction_CancelLoop::StartAction_Implementation(AActor* Instigator)
{
	UCActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.AppendTags(GrantTags);

	// 서버일 경우
	RepData.bIsRunning = true;
	RepData.Instigator = Instigator;

	UCAction* CancelAction = GetOwningComponent()->GetActionByName(CancelActionName);
	if (CancelAction) CancelAction->bCanCancelLoop = true;

	Comp->OnActionStopped.AddDynamic(this, &UCAction_CancelLoop::OnActionStopped);
}

void UCAction_CancelLoop::StopAction_Implementation(AActor* Instigator)
{
	
	UCActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.RemoveTags(GrantTags);

	// 서버일 경우
	RepData.bIsRunning = false;
	RepData.Instigator = Instigator;

}

bool UCAction_CancelLoop::CanStart_Implementation(AActor* Instigator, FString& OutMsg)
{
	UCAction* CancelAction = GetOwningComponent()->GetActionByName(CancelActionName);

	if (!CancelAction) return false;
	if (!CancelAction->IsRunning()) return false;

	return Super::CanStart_Implementation(Instigator, OutMsg);
}

void UCAction_CancelLoop::OnActionStopped(UCActionComponent* ActionComponent, UCAction* Action)
{
	StopAction_Implementation(GetOwningComponent()->GetOwner());
	ActionComponent->OnActionStopped.RemoveDynamic(this, &UCAction_CancelLoop::OnActionStopped);
}
