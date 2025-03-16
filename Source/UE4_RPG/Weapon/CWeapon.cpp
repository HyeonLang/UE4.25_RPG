// Fill out your copyright notice in the Description page of Project Settings.


#include "CWeapon.h"
#include "Global.h"
#include "Player/CPlayerController.h"
#include "Character/CPlayerCharacter.h"
#include "Character/CEnemyCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/CActionComponent.h"
#include "Components/CNPCActionComponent.h"
#include "Weapon/CWeapon.h"
#include "Weapon/CNPCWeapon.h"
#include "Actions/CAction.h"
#include "Actions/CNPCAction.h"

// Sets default values
ACWeapon::ACWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateSceneComponent(this, &RootComp, "RootComp");
	CHelpers::CreateSceneComponent(this, &CapsuleComp, "CapsuleComp", RootComp);
	CHelpers::CreateActorComponent(this, &SkeletalMeshComp, "SkeletalMeshComp");
	SkeletalMeshComp->AttachToComponent(CapsuleComp, FAttachmentTransformRules::KeepRelativeTransform);
	CHelpers::CreateSceneComponent(this, &MidComp, "MidComp", RootComp);
	CHelpers::CreateSceneComponent(this, &StartComp, "StartComp", RootComp);

	CapsuleComp->SetRelativeLocation(FVector(60, 0, 0));
	CapsuleComp->SetRelativeRotation(FRotator(90, 0, 0));
	CapsuleComp->SetCapsuleHalfHeight(90.f);

	InstigateAction = nullptr;
	AttackIndex = 0;

	SetReplicates(true);
	SetReplicateMovement(true);

	bAlwaysEquip = false;
	bMultiHitMode = false;
}

// Called when the game starts or when spawned
void ACWeapon::BeginPlay()
{
	Super::BeginPlay();

	if (SkeletalMeshComp)
		SkeletalMeshComp->SetVisibility(false);
	
	CapsuleComp->OnComponentBeginOverlap.AddDynamic(this, &ACWeapon::OnActorBeginOverlap);
	CapsuleComp->OnComponentEndOverlap.AddDynamic(this, &ACWeapon::OnActorEndOverlap);
	
	if (SkeletalMeshComp && TrailAnimSeq)
	{
		
		SkeletalMeshComp->SetAnimationMode(EAnimationMode::AnimationSingleNode);
		SkeletalMeshComp->PlayAnimation(TrailAnimSeq, true);
		SkeletalMeshComp->SetPlayRate(0.0);
	}

	if (bAlwaysEquip)
	{
		OnEquip();
	}
}

// Called every frame
void ACWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACWeapon::OnCollision(UCActionBase* NewAction, int32 NewAttackIndex)
{
	OverlappedActors.AddUnique(this);
	OverlappedActors.AddUnique(GetOwner());
	CapsuleComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	if (NewAction)
	{
		InstigateAction = NewAction;
		AttackIndex = NewAttackIndex;
	}

	// 이미 오버랩된 액터 감지
	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors);
	
	for (AActor* OverlappingActor : OverlappingActors)
	{
		if (OverlappingActor && OverlappingActor != this && OverlappingActor != GetOwner())
		{
			FHitResult HitResult = FHitResult();
			HitResult.Actor = OverlappingActor;
			HitResult.Location = OverlappingActor->GetActorLocation();
			UE_LOG(LogTemp, Warning, TEXT("Already overlapping with: %s"), *OverlappingActor->GetName());

			// 수동으로 오버랩 이벤트 실행
			OnActorBeginOverlap(CapsuleComp, OverlappingActor, nullptr, 0, false, HitResult);
			
		}
	}

	if(SkeletalMeshComp && TrailAnimSeq)
	{
		SkeletalMeshComp->SetPlayRate(1.0);
	}
	
}

void ACWeapon::OffCollision()
{
	if (SkeletalMeshComp && TrailAnimSeq)
	{
		SkeletalMeshComp->SetPlayRate(0.0);
	}

	CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	InstigateAction = nullptr;
	AttackIndex = 0;
	OverlappedActors.Empty();
}

bool ACWeapon::IsOverlappedActor(AActor* TargetActor)
{
	if (OverlappedActors.Find(TargetActor) != INDEX_NONE)
	{
		return true;
	}
	return false;
}

void ACWeapon::OnActorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (InstigateAction)
	{
		// 플레이어인 경우
		UCAction* Action = Cast<UCAction>(InstigateAction);
		if (Action)
		{
			if (!Cast<ACPlayerCharacter>(OtherActor) && !Cast<ACWeapon>(OtherActor) && !Cast<ACNPCWeapon>(OtherActor))
			{
				
				ACPlayerCharacter* PlayerCharacter = Cast<ACPlayerCharacter>(Action->GetOwningComponent()->GetOwner());
				if (PlayerCharacter)
				{
					InstigateAction->Attack_ElapsedByOverlapEvent(PlayerCharacter, this,  SweepResult, AttackIndex);
					OverlappedActors.AddUnique(OtherActor);
				}
			}
		}

		//NPC인 경우
		UCNPCAction* NPCAction = Cast<UCNPCAction>(InstigateAction);
		if (NPCAction)
		{
			ACEnemyCharacter* EnemyCharacter = Cast<ACEnemyCharacter>(NPCAction->GetOwningComponent()->GetOwner());
			if (!Cast<ACWeapon>(OtherActor) && !Cast<ACNPCWeapon>(OtherActor))
			{
				if (EnemyCharacter)
				{
					InstigateAction->Attack_ElapsedByOverlapEvent(EnemyCharacter, this, SweepResult, AttackIndex);
					OverlappedActors.AddUnique(OtherActor);
				}
			}
		}
	}
}



void ACWeapon::OnActorEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!bMultiHitMode) return;
	int32 Index = OverlappedActors.Find(OtherActor);
	if (Index != INDEX_NONE)
	{
		OverlappedActors.Remove(OtherActor);
	}
}

void ACWeapon::OnEquip_Implementation()
{
	if (SkeletalMeshComp)
	{
		SkeletalMeshComp->SetVisibility(true);
	}
	
}

void ACWeapon::OnUnequip_Implementation()
{
	if (bAlwaysEquip) return;

	if (SkeletalMeshComp)
	{
		SkeletalMeshComp->SetVisibility(false);
	}
}

void ACWeapon::ActorAttachTo_Implementation(FName InSoketName)
{
	AttachToComponent
	(
		Cast<ACharacter>(GetOwner())->GetMesh(),
		FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true),
		//FAttachmentTransformRules(EAttachmentRule::KeepRelative, true),
		InSoketName
	);
}

void ACWeapon::ComponentAttachTo_Implementation(USceneComponent* InComponent, FName InSoketName)
{
	InComponent->AttachToComponent
	(
		Cast<ACharacter>(GetOwner())->GetMesh(),
		FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true),
		//FAttachmentTransformRules(EAttachmentRule::KeepRelative, true),
		InSoketName
	);
}

