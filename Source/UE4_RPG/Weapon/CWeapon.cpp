// Fill out your copyright notice in the Description page of Project Settings.


#include "CWeapon.h"
#include "Global.h"
#include "Character/CPlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/CActionComponent.h"
#include "Actions/CAction.h"

// Sets default values
ACWeapon::ACWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateSceneComponent(this, &RootComp, "RootComp");
	CHelpers::CreateActorComponent(this, &SkeletalMeshComp, "SkeletalMeshComp");
	SkeletalMeshComp->AttachToComponent(RootComp, FAttachmentTransformRules::KeepRelativeTransform);
	CHelpers::CreateSceneComponent(this, &CapsuleComp, "CapsuleComp", RootComp);
	CHelpers::CreateSceneComponent(this, &MidComp, "MidComp", RootComp);
	CHelpers::CreateSceneComponent(this, &StartComp, "StartComp", RootComp);

	CapsuleComp->SetRelativeLocation(FVector(60, 0, 0));
	CapsuleComp->SetRelativeRotation(FRotator(90, 0, 0));
	CapsuleComp->SetCapsuleHalfHeight(90.f);

	InstigateAction = nullptr;
	AttackIndex = 0;

	SetReplicates(true);
	SetReplicateMovement(true);
}

// Called when the game starts or when spawned
void ACWeapon::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
	SkeletalMeshComp->SetVisibility(false);
	
	CapsuleComp->OnComponentBeginOverlap.AddDynamic(this, &ACWeapon::OnActorBeginOverlap);
	CapsuleComp->OnComponentEndOverlap.AddDynamic(this, &ACWeapon::OnActorEndOverlap);

	//OffCollision();
}

// Called every frame
void ACWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACWeapon::OnCollision(UCAction* NewAction, int32 NewAttackIndex)
{
	CapsuleComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	if (NewAction)
	{
		InstigateAction = NewAction;
		AttackIndex = NewAttackIndex;
	}
}

void ACWeapon::OffCollision()
{
	CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	InstigateAction = nullptr;
	AttackIndex = 0;
	OverlappedActors.Empty();
}

bool ACWeapon::IsOverlappedActor(AActor* TargetActor)
{
	if (OverlappedActors.Find(TargetActor) == -1)
	{
		return true;
	}
	return false;
}

void ACWeapon::OnActorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (InstigateAction)
	{
		ACPlayerCharacter* PlayerCharacter = Cast<ACPlayerCharacter>(InstigateAction->GetOwningComponent()->GetOwner());
		if (PlayerCharacter)
		{
			InstigateAction->Attack_ElapsedByOverlapEvent(PlayerCharacter, this,  SweepResult, AttackIndex);
		}
	}

	OverlappedActors.AddUnique(OtherActor);
}

void ACWeapon::OnActorEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	int32 Index = OverlappedActors.Find(OtherActor);
	if (Index != INDEX_NONE)
	{
		OverlappedActors.Remove(OtherActor);
	}
}

void ACWeapon::OnEquip_Implementation()
{
	if (ensure(SkeletalMeshComp))
	{
		SkeletalMeshComp->SetVisibility(true);
	}
	
}

void ACWeapon::OnUnequip_Implementation()
{
	if (ensure(SkeletalMeshComp))
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

