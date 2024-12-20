#include "CNPCActionComponent.h"

#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"

#include "Global.h"
#include "Actions/CNPCAction.h"
#include "Game/CCooldownManager.h"

DECLARE_CYCLE_STAT(TEXT("StartNPCActionByName"), STAT_StartNPCActionByName, STATGROUP_TORE);

UCNPCActionComponent::UCNPCActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bCanStopMontagePostNPCAction = false;
	// 컴포넌트 리플리케이트
	SetIsReplicatedByDefault(true);
}


void UCNPCActionComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

void UCNPCActionComponent::BeginPlay()
{
	Super::BeginPlay();

	// 서버 캐릭터들에서만 액션 등록 후 리플리케이트
	if (GetOwner()->HasAuthority()) // AddAction 위치가 Authority
	{
		for (TSubclassOf<UCNPCAction> ActionClass : DefaultNPCActions)
		{
			AddNPCAction(GetOwner(), ActionClass);
		}
	}

	if (OnNPCActionCreateFinished.IsBound())
	{
		OnNPCActionCreateFinished.Broadcast(this);
	}
}


void UCNPCActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	for (UCNPCAction* NPCAction : NPCActions)
	{
		if (NPCAction)
			NPCAction->NPCActionTick(DeltaTime);
	}
}


bool UCNPCActionComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool bChangedSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for (UCNPCAction* NPCAction : NPCActions)
	{
		if (NPCAction)
		{
			bChangedSomething |= Channel->ReplicateSubobject(NPCAction, *Bunch, *RepFlags);

			if (NPCAction->CooldownManager)
			{
				bChangedSomething |= Channel->ReplicateSubobject(NPCAction->CooldownManager, *Bunch, *RepFlags);
			}
		}
	}

	return bChangedSomething;
}

void UCNPCActionComponent::AddNPCAction(AActor* Instigator, TSubclassOf<UCNPCAction> NPCActionClass)
{
	if (!ensure(NPCActionClass))
	{
		return;
	}

	if (!GetOwner()->HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Client attempting to AddAction. [Class : %s]"), *GetNameSafe(NPCActionClass));
		return;
	}

	UCNPCAction* NewAction = NewObject<UCNPCAction>(GetOwner(), NPCActionClass);
	if (ensure(NewAction))
	{
		NewAction->SetOwningComponent(this);
		NewAction->Initialize();

		NPCActions.Add(NewAction); // 서버에서 등록 한 후 Actions 리플리케이트

		FString msg;
		if (NewAction->bAutoStart && ensure(NewAction->CanStart(Instigator, msg)))
		{
			NewAction->StartNPCAction(Instigator);
		}
	}
}

UCNPCAction* UCNPCActionComponent::GetNPCAction(TSubclassOf<UCNPCAction> NPCActionClass) const
{
	for (UCNPCAction* NPCAction : NPCActions)
	{
		if (NPCAction && NPCAction->IsA(NPCActionClass))
		{
			return NPCAction;
		}
	}

	return nullptr;
}

UCNPCAction* UCNPCActionComponent::GetNPCActionByName(FName InNPCActionName) const
{
	for (UCNPCAction* NPCAction : NPCActions)
	{
		if (NPCAction && NPCAction->NPCActionName == InNPCActionName)
		{
			return NPCAction;
		}
	}

	return nullptr;
}

void UCNPCActionComponent::RemoveNPCAction(UCNPCAction* NPCActionToRemove)
{
	if (!ensure(NPCActionToRemove && !NPCActionToRemove->IsRunning()))
	{
		return;
	}


	NPCActions.Remove(NPCActionToRemove);
}

bool UCNPCActionComponent::StartNPCActionByName(AActor* Instigator, FName NPCActionName)
{
	SCOPE_CYCLE_COUNTER(STAT_StartNPCActionByName); // 인사이트 성능 확인용

	for (UCNPCAction* NPCAction : NPCActions)
	{
		if (NPCAction && NPCAction->NPCActionName == NPCActionName)
		{


			FString Msg;
			if (!NPCAction->CanStart(Instigator, Msg))
			{
				FString Message = FString::Printf(TEXT("Faild to run : %s :"), *NPCActionName.ToString());
				Message = Message + Msg;
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, Message);
				continue;
			}

			// 1.로컬에서 실행시키고 2. 서버로 보냄 (조작감)
			if (!GetOwner()->HasAuthority())
			{
				// 서버에서 실행될 때는 서버에서는 여기로 들어오지 않음.
				ServerStartNPCAction(Instigator, NPCActionName);
			}

			TRACE_BOOKMARK(TEXT("StartNPCAction::%s"), *GetNameSafe(NPCAction));

			{
				NPCAction->StartNPCAction(Instigator);

				SCOPED_NAMED_EVENT_FSTRING(NPCAction->GetClass()->GetName(), FColor::White);
			}

			return true;
		}
	}

	return false;
}


void UCNPCActionComponent::ServerStartNPCAction_Implementation(AActor* Instigator, FName NPCActionName)
{
	StartNPCActionByName(Instigator, NPCActionName);
}


bool UCNPCActionComponent::StopNPCActionByName(AActor* Instigator, FName NPCActionName)
{
	for (UCNPCAction* NPCAction : NPCActions)
	{
		if (NPCAction && NPCAction->NPCActionName == NPCActionName)
		{
			if (NPCAction->IsRunning())
			{
				// 1.로컬에서 실행시키고 2. 서버로 보냄 (조작감)
				if (!GetOwner()->HasAuthority())
				{
					// 서버에서 실행되지만 서버에서는 여기로 들어오지 않음.
					ServerStopNPCAction(Instigator, NPCActionName);
				}

				NPCAction->StopNPCAction(Instigator);
				return true;
			}
		}
	}

	return false;
}

void UCNPCActionComponent::ServerStopNPCAction_Implementation(AActor* Instigator, FName NPCActionName)
{
	StopNPCActionByName(Instigator, NPCActionName);
}

void UCNPCActionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCNPCActionComponent, NPCActions);
}