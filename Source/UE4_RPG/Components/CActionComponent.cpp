#include "CActionComponent.h"

#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"

#include "Global.h"
#include "Actions/CAction.h"


DECLARE_CYCLE_STAT(TEXT("StartActionByName"), STAT_StartActionByName, STATGROUP_TORE);


UCActionComponent::UCActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bCanStopMontagePostAction = false;
	// 컴포넌트 리플리케이트
	SetIsReplicatedByDefault(true);
}


void UCActionComponent::BeginPlay()
{
	Super::BeginPlay();

	// 서버 캐릭터들에서만 액션 등록 후 리플리케이트
	if (GetOwner()->HasAuthority()) // AddAction 위치가 Authority
	{
		for (TSubclassOf<UCAction> ActionClass : DefaultActions)
		{
			AddAction(GetOwner(), ActionClass); 
		}
	}
}


void UCActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	/*FString Message = GetNameSafe(GetOwner()) + " : " + ActiveGameplayTags.ToStringSimple();
	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::White, Message);*/

	for (UCAction* Action : Actions)
	{
		if (Action)
			Action->ActionTick(DeltaTime);
	}
	
}

bool UCActionComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool bChangedSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for (UCAction* Action : Actions)
	{
		if (Action)
		{
			bChangedSomething |= Channel->ReplicateSubobject(Action, *Bunch, *RepFlags);
		}
	}

	return bChangedSomething;
}


void UCActionComponent::AddAction(AActor* Instigator, TSubclassOf<UCAction> ActionClass)
{
	if (!ensure(ActionClass))
	{
		return;
	}

	if (!GetOwner()->HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Client attempting to AddAction. [Class : %s]"), *GetNameSafe(ActionClass));
		return;
	}

	UCAction* NewAction = NewObject<UCAction>(GetOwner(), ActionClass);
	if (ensure(NewAction))
	{
		NewAction->SetOwningComponent(this);
		NewAction->Initialize();

		Actions.Add(NewAction); // 서버에서 등록 한 후 Actions 리플리케이트

		FString msg;
		if (NewAction->bAutoStart && ensure(NewAction->CanStart(Instigator, msg)))
		{
			NewAction->StartAction(Instigator);
		}
	}
}

UCAction* UCActionComponent::GetAction(TSubclassOf<UCAction> ActionClass) const
{
	for (UCAction* Action : Actions)
	{
		if (Action && Action->IsA(ActionClass))
		{
			return Action;
		}
	}

	return nullptr;
}

UCAction* UCActionComponent::GetActionByName(FName InActionName) const
{
	for (UCAction* Action : Actions)
	{
		if (Action && Action->ActionName == InActionName)
		{
			return Action;
		}
	}

	return nullptr;
}

void UCActionComponent::RemoveAction(UCAction* ActionToRemove)
{
	if (!ensure(ActionToRemove && !ActionToRemove->IsRunning()))
	{
		return;
	}


	Actions.Remove(ActionToRemove);
}

bool UCActionComponent::StartActionByName(AActor* Instigator, FName ActionName, bool bBaseAction)
{
	SCOPE_CYCLE_COUNTER(STAT_StartActionByName); // 인사이트 성능 확인용

	for (UCAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			if (Action->GetCanCombo()) // NextCombo
			{
				FName CurrentComboActionName = Action->CurrentComboActionName;
				Action->SetCanCombo(false);

				UCAction* CurrentComboAction = GetActionByName(CurrentComboActionName);

				if (CurrentComboAction->IsRunning())
				{
					StopActionByName(Instigator, CurrentComboAction->ActionName);
				}

				Action->CurrentComboActionName = CurrentComboAction->NextComboActionName;

				StartActionByName(Instigator, Action->CurrentComboActionName, false);


				if (GetActionByName(Action->CurrentComboActionName)->NextComboActionName == "None")
				{
					Action->CurrentComboActionName = ActionName;
				}

				return false;
			}

			FString Msg;
			if (!Action->CanStart(Instigator, Msg))
			{
				FString Message = FString::Printf(TEXT("Faild to run : %s :"), *ActionName.ToString());
				Message = Message + Msg;
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, Message);
				continue;
			}

			if (bBaseAction)
			{
				ComboReset(Action, ActionName);
			}

			// 1.로컬에서 실행시키고 2. 서버로 보냄 (조작감)
			if (!GetOwner()->HasAuthority())
			{
				// 서버에서 실행될 때는 서버에서는 여기로 들어오지 않음.
				ServerStartAction(Instigator, ActionName);
			}

			TRACE_BOOKMARK(TEXT("StartAction::%s"), *GetNameSafe(Action));

			{
				Action->StartAction(Instigator);

				SCOPED_NAMED_EVENT_FSTRING(Action->GetClass()->GetName(), FColor::White);
			}

			return true;
		}
	}

	return false;
}

void UCActionComponent::ServerStartAction_Implementation(AActor* Instigator, FName ActionName)
{
	StartActionByName(Instigator, ActionName);
}



bool UCActionComponent::StopActionByName(AActor* Instigator, FName ActionName)
{
	for (UCAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			if (Action->IsRunning())
			{
				// 1.로컬에서 실행시키고 2. 서버로 보냄 (조작감)
				if (!GetOwner()->HasAuthority())
				{
					// 서버에서 실행되지만 서버에서는 여기로 들어오지 않음.
					ServerStopAction(Instigator, ActionName);
				}

				Action->StopAction(Instigator);
				return true;
			}
		}
	}

	return false;
}

void UCActionComponent::ComboReset(UCAction* InAction, FName ResetName)
{
	InAction->CurrentComboActionName = ResetName;
}

void UCActionComponent::ServerStopAction_Implementation(AActor* Instigator, FName ActionName)
{
	StopActionByName(Instigator, ActionName);
}

void UCActionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCActionComponent, Actions);
}