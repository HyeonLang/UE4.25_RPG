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

	
	MeshComp->SetSimulatePhysics(true);
}

void ACItemBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

