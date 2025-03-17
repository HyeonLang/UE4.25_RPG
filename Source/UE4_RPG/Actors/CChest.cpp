
#include "CChest.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Net/UnrealNetwork.h"

#include "Global.h"
#include "Actors/Item/CItemManager.h"

ACChest::ACChest()
{
	CHelpers::CreateSceneComponent<UBoxComponent>(this, &BoxComp, "BoxComp", RootComponent);

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	BaseMesh->SetupAttachment(BoxComp);

	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>("LidMesh");
	LidMesh->SetupAttachment(BaseMesh);

	TreasureMesh = CreateDefaultSubobject<UStaticMeshComponent>("TreasureMesh");
	TreasureMesh->SetupAttachment(BaseMesh);

	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");
	EffectComp->SetupAttachment(BaseMesh);

	MaxPitch = 110.f;

	PrimaryActorTick.bCanEverTick = true;

	bCanOpen = false;

	SetReplicates(true);
}


void ACChest::BeginPlay()
{
	Super::BeginPlay();

	if (EffectComp) EffectComp->SetActive(!bCanOpen);


	// Create Dynamic Material
	for (int32 i = 0; i < BaseMesh->GetMaterials().Num(); i++)
	{
		UMaterialInterface* Material = BaseMesh->GetMaterial(i);
		UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(Material, this);
		if (ensure(DynamicMaterial))
		{
			BaseMesh->SetMaterial(i, DynamicMaterial);
		}
	}

	// Create Dynamic Material
	for (int32 i = 0; i < TreasureMesh->GetMaterials().Num(); i++)
	{
		UMaterialInterface* Material = TreasureMesh->GetMaterial(i);
		UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(Material, this);
		if (ensure(DynamicMaterial))
		{
			TreasureMesh->SetMaterial(i, DynamicMaterial);
		}
	}

	// Create Dynamic Material
	for (int32 i = 0; i < LidMesh->GetMaterials().Num(); i++)
	{
		UMaterialInterface* Material = LidMesh->GetMaterial(i);
		UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(Material, this);
		if (ensure(DynamicMaterial))
		{
			LidMesh->SetMaterial(i, DynamicMaterial);
		}
	}
}


void ACChest::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!bCanOpen) return;

	bLidOpen = !bLidOpen;
	OnRep_LidOpen();

	if (bLidOpen)
	{
		SpawnItems(InstigatorPawn);
	}
}

FName ACChest::GetInteractName_Implementation(APawn* InstigatorPawn)
{
	return FName("Chest");
}

void ACChest::DestroyAction_Implementation()
{
	if (HasAuthority())
	{
		Destroy();
	}
}

void ACChest::SetCanOpen_Implementation(bool bInNew)
{
	bCanOpen = bInNew;
	OnRep_CanOpen();
}

void ACChest::SpawnItems_Implementation(APawn* InstigatorPawn)
{
}

void ACChest::OnRep_CanOpen()
{
	if (EffectComp) EffectComp->SetActive(!bCanOpen);
}

void ACChest::OnRep_LidOpen()
{
	float CurrentPitch = bLidOpen ? MaxPitch : 0.f;
	CLog::Print(CurrentPitch);
	if (LidMesh)
	{
		LidMesh->SetRelativeRotation(FRotator(CurrentPitch, 0, 0));
	}

	DestroyAction();
}


void ACChest::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACChest, bLidOpen);
	DOREPLIFETIME(ACChest, bCanOpen);
}