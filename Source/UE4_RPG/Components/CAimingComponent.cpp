#include "CAimingComponent.h"
#include "../Character/CPlayerCharacter.h"
#include "../Player/CPlayerController.h"
#include "../Player/CPlayerCameraActor.h"

UCAimingComponent::UCAimingComponent()
{
	SetIsReplicatedByDefault(true);

	DistanceWeight = 1.f;
	CameraDirectionWeight = 1.f;

	DotBaseScore = 1.f;
	DistanceBaseScore = 1.f;
}


void UCAimingComponent::BeginPlay()
{
	Super::BeginPlay();

	DistanceWeight = FMath::Clamp(DistanceWeight, 0.f, 1.0f);
	CameraDirectionWeight = FMath::Clamp(CameraDirectionWeight, 0.f, 1.0f);
}

void UCAimingComponent::SetDistanceWeight(float InValue)
{
	DistanceWeight = InValue;
}

void UCAimingComponent::SetCameraDirectionWeight(float InValue)
{
	CameraDirectionWeight = InValue;
}

void UCAimingComponent::GetAimTargetDirection(FRotator& OutDirection, AActor* OutTarget, const float InRange, const bool InIsBossMode)
{
	AActor* Player = GetOwner();

	if (ensure(Player))
	{
		TArray<FOverlapResult> OverlapResults;

		FCollisionObjectQueryParams ObjectQueryParams;
		ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn); 
		// Todo. 공격 상호작용 액터 ECollisionChannel 추가

		FCollisionQueryParams CollisionQueryParams;
		CollisionQueryParams.AddIgnoredActor(Player);

		if (GetWorld()->OverlapMultiByObjectType(OverlapResults, Player->GetActorLocation(), Player->GetActorRotation().Quaternion(), ObjectQueryParams, FCollisionShape::MakeSphere(InRange), CollisionQueryParams))
		{
			
			std::priority_queue<std::pair<float, AActor*>> TargetDatas;

			for (int i = 0; i < OverlapResults.Num(); i++)
			{
				AActor* Target = OverlapResults[i].GetActor();
				FVector Direction = Target->GetActorLocation() - Player->GetActorLocation();

				float Dot;
				float Distance;
				Dot = FVector::DotProduct(Player->GetActorForwardVector().GetSafeNormal(), Direction.GetSafeNormal());
				Distance = Player->GetDistanceTo(Target);

				ACPlayerCharacter* P = Cast<ACPlayerCharacter>(Player);
				if (P && P->GetController())
				{
					ACPlayerController* PC = Cast<ACPlayerController>(P->GetController());
					if (PC && PC->GetPlayerCameraActor())
					{
						Dot = FVector::DotProduct(PC->GetPlayerCameraActor()->GetActorForwardVector().GetSafeNormal(), Direction.GetSafeNormal());
					}
				}

				float Score = CalcWeight(Dot, Distance, InRange);

				if (InIsBossMode)
				{
					//Todo. Actor가 보스이면 보스 베이스스코어 더해주기
				}
				//Todo. 공격 상호작용이 아닌 것들은 베이스스코어를 만들어서 빼주기

				std::pair<float, AActor*> Pair = std::make_pair(Score, Target);
				TargetDatas.push(Pair);
			}
			
			OutTarget = TargetDatas.top().second;
			OutDirection = (OutTarget->GetActorLocation() - Player->GetActorLocation()).Rotation();
		}
		else
		{
			OutDirection = Player->GetActorRotation();
			OutTarget = nullptr;
		}
	}
}

float UCAimingComponent::CalcWeight(float Dot, float Distance, float InRange)
{
	float DotScore = Dot * DotBaseScore * (CameraDirectionWeight / (DistanceWeight + CameraDirectionWeight));
	float DistanceScore = (1.f - (Distance / InRange)) * DistanceBaseScore * (DistanceWeight / (DistanceWeight + CameraDirectionWeight));

	return DotScore + DistanceScore;
}



