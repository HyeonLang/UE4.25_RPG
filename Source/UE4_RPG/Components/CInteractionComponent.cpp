
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
	bCanInteractByCooldown = true;

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
	bool bDrawDebug = false;// CVarDrawDebug.GetValueOnGameThread();


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

	// ������ ����Ʈ ������ �Ųٷ� ��ȸ
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


	if (bBlockingHit)
	{
		for (const auto& Hit : HitResults)
		{
			if (bDrawDebug)
			{
				DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 20, 20, FColor::Green, false, 0.f);
			}

			AActor* HitActor = Hit.GetActor();
			if (HitActor)
			{
				if (HitActor->Implements<UCInteractionInterface>() && !FocusedActors.Contains(HitActor))
				{
					
					FocusedActors.Add(HitActor);

					if (OnAddFocusedActor.IsBound())
					{
						OnAddFocusedActor.Broadcast(this, HitActor);
					}
					
				}
			}
		}

	}

	/*FString Str = "Hit : ";
	for (AActor* A : FocusedActors)
		Str = Str + " " + A->GetName();
	CLog::Print(Str, -1, 0.f);*/
}

void UCInteractionComponent::OnCooldownComplete()
{
	bCanInteractByCooldown = true;
}

void UCInteractionComponent::PrimaryInteraction()
{
	if (!bCanInteractByCooldown) return;
	// ���� ������ �Ű������� ������ �Ѱ� ���� rpc �Լ��� ����
	// RPC �̸鼭 ���͸� ��ӹ��� �����͸� �ѱ�� ������(ä��)���� �ٲ㼭 �ѱ��.
	if (FocusedActors.Num() < 1) return;
	ServerInteract(FocusedActors[0]);

	// Set interaction cooldown
	bCanInteractByCooldown = false;
	FTimerHandle InteractTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(InteractTimerHandle, this, &UCInteractionComponent::OnCooldownComplete, 1.0f, false);
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

