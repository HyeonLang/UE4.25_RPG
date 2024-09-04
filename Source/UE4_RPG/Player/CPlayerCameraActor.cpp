#include "CPlayerCameraActor.h"
#include "../Global.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SceneComponent.h"

ACPlayerCameraActor::ACPlayerCameraActor()
{
	CHelpers::CreateSceneComponent(this, &SceneComp, "SceneComp", RootComponent);
	CHelpers::CreateSceneComponent(this, &SpringArmComp, "SpringArmComp", SceneComp);
	CHelpers::CreateSceneComponent(this, &CameraComp, "CameraComp", SpringArmComp);

	//-> SpringArmComp
	SpringArmComp->TargetArmLength = 500.0f;
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->bEnableCameraLag = true;

	SetReplicates(true);
	SetReplicateMovement(true);
}

void ACPlayerCameraActor::BeginPlay()
{
	Super::BeginPlay();
}


