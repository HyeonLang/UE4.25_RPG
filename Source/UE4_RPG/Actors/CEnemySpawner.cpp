// Fill out your copyright notice in the Description page of Project Settings.


#include "CEnemySpawner.h"
#include "Global.h"
#include "Character/CPlayerCharacter.h"

static EDrawDebugTrace::Type DrawDebugTrace = EDrawDebugTrace::ForDuration;

ACEnemySpawner::ACEnemySpawner()
{
	
	AttackModeRange = 800;
	AttackModeReleaseRange = 1500;
	bIsAttackMode = false;
	SetActorTickInterval(0.1f);
}

void ACEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
}

void ACEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TargetActors = TraceMultiForTargets(AttackModeReleaseRange);

	if (TargetActors.Num() < 1)
	{
		bIsAttackMode = false;
	}
	else
	{
		if (TraceMultiForTargets(AttackModeRange).Num() > 0) bIsAttackMode = true;
	}

}

bool ACEnemySpawner::IsAttackMode() const
{
	return bIsAttackMode;
}

AActor* ACEnemySpawner::FindNearestTarget(AActor* InActor)
{
	float MinDistance = AttackModeReleaseRange + 1.f;
	AActor* NearestTarget = nullptr;

	for (AActor* TargetActor : TargetActors)
	{
		float Distance = UKismetMathLibrary::Vector_Distance(InActor->GetActorLocation(), TargetActor->GetActorLocation());
		if (MinDistance > Distance)
		{
			MinDistance = Distance;
			NearestTarget = TargetActor;
		}
	}

	return NearestTarget;
}

TArray<AActor*> ACEnemySpawner::TraceMultiForTargets(float Radius)
{
	TArray<FHitResult> HitResults;
	TArray<AActor*> TraceTargets;

	bool bHit = UKismetSystemLibrary::SphereTraceMultiForObjects(
		GetWorld(),
		GetActorLocation(),
		GetActorLocation(),
		Radius,
		TargetObjectTypes,
		false,
		TargetActorIgnores,
		DrawDebugTrace,
		HitResults,
		true
	);

	float MinDistance = Radius + 1.f;

	if (bHit)
	{
		for (FHitResult HitResult : HitResults)
		{
			ACPlayerCharacter* PC = Cast<ACPlayerCharacter>(HitResult.GetActor());
			if (!PC) continue;

			if (PC->GetController())
			{
				TraceTargets.AddUnique(PC);
			}
		}

	}

	return TraceTargets;
}


