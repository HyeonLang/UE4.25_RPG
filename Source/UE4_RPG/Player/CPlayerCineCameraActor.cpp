#include "CPlayerCineCameraActor.h"
#include "Global.h"

#include "CineCameraComponent.h"
#include "Components/SceneComponent.h"

ACPlayerCineCameraActor::ACPlayerCineCameraActor()
{
	CHelpers::CreateSceneComponent(this, &SceneComp, "SceneComp", RootComponent);
	CHelpers::CreateSceneComponent(this, &CineCameraComp, "CineCameraComp", SceneComp);

	CineCameraComp->CurrentAperture = 22.f;
	PrimaryActorTick.bCanEverTick = true;
}

void ACPlayerCineCameraActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACPlayerCineCameraActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ACPlayerCineCameraActor::PlayCinematic_Implementation()
{
	
}

void ACPlayerCineCameraActor::PlayCinematicFinished_Implementation()
{
	if (OnPlayCinematicFinished.IsBound())
	{
		OnPlayCinematicFinished.Broadcast(this);
	}
}