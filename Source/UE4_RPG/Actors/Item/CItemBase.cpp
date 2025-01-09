#include "CItemBase.h"
#include "Net/UnrealNetwork.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"

#include "Global.h"


ACItemBase::ACItemBase()
{
	PrimaryActorTick.bCanEverTick = true;
	SetReplicates(true);
	SetReplicateMovement(true);

	CHelpers::CreateSceneComponent<UStaticMeshComponent>(this, &MeshComp, "MeshComp", RootComponent);
	CHelpers::CreateSceneComponent<USphereComponent>(this, &SphereComp, "SphereComp", MeshComp);
	CHelpers::CreateSceneComponent<UParticleSystemComponent>(this, &EffectComp, "EffectComp", MeshComp);

	SphereComp->SetSphereRadius(50.f);
	MeshComp->SetSimulatePhysics(true);
}

void ACItemBase::BeginPlay()
{
	Super::BeginPlay();
	
	SetLifeSpan(30.f);
}

void ACItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACItemBase::Interact_Implementation(APawn* InstigatorPawn)
{
}

FText ACItemBase::GetInteractText_Implementation(APawn* InstigatorPawn)
{
	return ItemName;
}

