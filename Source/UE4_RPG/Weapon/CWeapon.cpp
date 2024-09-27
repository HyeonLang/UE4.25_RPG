// Fill out your copyright notice in the Description page of Project Settings.


#include "CWeapon.h"
#include "Global.h"
#include "Character/CPlayerCharacter.h"

// Sets default values
ACWeapon::ACWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateSceneComponent(this, &RootComp, "RootComp");
	CHelpers::CreateActorComponent(this, &SkeletalMeshComp, "SkeletalMeshComp");
	SkeletalMeshComp->AttachToComponent(RootComp, FAttachmentTransformRules::KeepRelativeTransform);

	SetReplicates(true);
	SetReplicateMovement(true);
}

// Called when the game starts or when spawned
void ACWeapon::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
	SkeletalMeshComp->SetVisibility(false);
}

// Called every frame
void ACWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
		OwnerCharacter->GetMesh(),
		FAttachmentTransformRules(EAttachmentRule::KeepRelative, true),
		InSoketName
	);
}

void ACWeapon::ComponentAttachTo_Implementation(USceneComponent* InComponent, FName InSoketName)
{
	InComponent->AttachToComponent
	(
		OwnerCharacter->GetMesh(),
		FAttachmentTransformRules(EAttachmentRule::KeepRelative, true),
		InSoketName
	);
}

