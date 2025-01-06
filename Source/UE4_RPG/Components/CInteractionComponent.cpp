
#include "CInteractionComponent.h"

#include "Global.h"
#include "Interfaces/CInteractionInterface.h"
#include "Character/CPlayerCharacter.h"

static TAutoConsoleVariable<bool> CVarDrawDebug(TEXT("RPG.DrawDebug"), false, TEXT("Enable DrawDebug for interaction"), ECVF_Cheat);


UCInteractionComponent::UCInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	TraceRadius = 300.f;
	TraceAngle = 90.f;

	bInteractionMode = true;

	CollisionChannels.Add(ECC_WorldDynamic);
}


void UCInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
	
}



void UCInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bInteractionMode) return;

	ACPlayerCharacter* PlayerCharacter = Cast<ACPlayerCharacter>(GetOwner());
	if (!PlayerCharacter->GetOnField()) return;

	//Local
	if (PlayerCharacter && PlayerCharacter->IsLocallyControlled())
	{
		FindNearestInteractable();
	}
}

void UCInteractionComponent::SetInteractionMode(bool NewMode)
{
	bInteractionMode = NewMode;
}

void UCInteractionComponent::FindNearestInteractable()
{
	bool bDrawDebug = true;// CVarDrawDebug.GetValueOnGameThread();


	TArray<FHitResult> HitResults;

	FCollisionObjectQueryParams ObjectQueryParams;
	for (TEnumAsByte<ECollisionChannel> CollisionChannel : CollisionChannels)
	{
		ObjectQueryParams.AddObjectTypesToQuery(CollisionChannel);
	}
	
	FCollisionShape Shape;
	Shape.SetSphere(TraceRadius);

	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(
		HitResults,
		GetOwner()->GetActorLocation(),
		GetOwner()->GetActorLocation(),
		FQuat::Identity,
		ObjectQueryParams,
		Shape
	);

	// 삭제시 쉬프트 때문에 거꾸로 순회
	for (int32 i = FocusedActors.Num() - 1; i >= 0; i--)
	{
		bool bActiveActor = false;
		AActor* FocusedActor = FocusedActors[i];

		for (const auto& Hit : HitResults)
		{
			if (FocusedActor == Hit.GetActor())
			{
				bActiveActor = true;
				break;
			}
		}

		if (!bActiveActor)
		{
			FocusedActors.RemoveAt(i);

			if (OnRemoveFocusedActor.IsBound())
			{
				OnRemoveFocusedActor.Broadcast(this, FocusedActor);
			}
		}
	}

	FString Str = "Hit : ";

	if (bBlockingHit)
	{
		for (const auto& Hit : HitResults)
		{
			if (bDrawDebug)
			{
				DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 20, 20, FColor::Green, false, 0.f);
			}

			AActor* HitActor = Hit.GetActor();
			Str = Str + " " + Hit.GetActor()->GetName();
			if (HitActor)
			{
				if (HitActor->Implements<UCInteractionInterface>())
				{

					FocusedActors.AddUnique(HitActor);

					if (OnAddFocusedActor.IsBound())
					{
						OnAddFocusedActor.Broadcast(this, HitActor);
					}
				}
			}
		}

	}

	CLog::Print(Str, -1, 0.f);
}

void UCInteractionComponent::PrimaryInteraction()
{
	// 로컬 변수를 매개변수로 서버로 넘겨 서버 rpc 함수를 실행
	// RPC 이면서 액터를 상속받은 포인터를 넘기면 도메인(채널)으로 바꿔서 넘긴다.
	ServerInteract(FocusedActors[0]);
}

void UCInteractionComponent::ServerInteract_Implementation(AActor* InFocused)
{
	if (InFocused == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Orange, "There is no interable object.");
		return;
	}

	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	ICInteractionInterface::Execute_Interact(InFocused, OwnerPawn);
}

