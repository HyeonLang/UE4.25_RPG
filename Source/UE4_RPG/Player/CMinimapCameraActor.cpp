#include "CMinimapCameraActor.h"
#include "../Global.h"
#include "CPlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Components/SceneComponent.h"
#include "Engine/TextureRenderTarget2D.h"
#include "PaperSpriteComponent.h"



ACMinimapCameraActor::ACMinimapCameraActor()
{
	CHelpers::CreateSceneComponent(this, &SceneComp, "SceneComp", RootComponent);
	CHelpers::CreateSceneComponent(this, &PlayerMarkerSpriteComp, "PlayerMarkerSpriteComp", RootComponent);
	CHelpers::CreateSceneComponent(this, &SpringArmComp, "SpringArmComp", SceneComp);
	CHelpers::CreateSceneComponent(this, &SceneCaptureComp, "SceneCaptureComp", SpringArmComp);
	CHelpers::CreateSceneComponent(this, &DepthCaptureComp, "DepthCaptureComp", SpringArmComp);

	
	PlayerMarkerSpriteComp->bOwnerNoSee = true;
	PlayerMarkerSpriteComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);


	//-> SpringArmComp
	SpringArmComp->TargetArmLength = 1000.0f;
	SpringArmComp->SetWorldRotation(FRotator(-90.f, 0.f, 0.f));
	SpringArmComp->bUsePawnControlRotation = false;
	SpringArmComp->bInheritPitch = false;
	SpringArmComp->bInheritRoll = false;
	SpringArmComp->bInheritYaw = false;

	//카메라 투영 타입을 직교로 전환하여 거리감이 없는 이미지로 구현
	SceneCaptureComp->ProjectionType = ECameraProjectionMode::Orthographic;
	//카메라에서 캡처될 크기 ( 클수록 축소되 보이는 미니맵 )
	SceneCaptureComp->OrthoWidth = 3072;

	SceneCaptureComp->CaptureSource = ESceneCaptureSource::SCS_SceneColorSceneDepth;
	SceneCaptureComp->ShowFlags.SetSkeletalMeshes(false);
	SceneCaptureComp->ShowFlags.SetParticles(false);
	SceneCaptureComp->ShowFlags.SetDynamicShadows(false);
	SceneCaptureComp->ShowFlags.SetCollisionPawn(false);
	SceneCaptureComp->ShowFlags.SetPostProcessing(true);   // 포스트 프로세싱 활성화 
	SceneCaptureComp->ShowFlags.SetSkyLighting(false);
	SceneCaptureComp->ShowFlags.SetWidgetComponents(false);
	SceneCaptureComp->ShowFlags.SetSingleFlag(FEngineShowFlags::EShowFlag::SF_ContactShadows, false);

	//카메라 투영 타입을 직교로 전환하여 거리감이 없는 이미지로 구현
	DepthCaptureComp->ProjectionType = ECameraProjectionMode::Orthographic;
	//카메라에서 캡처될 크기 ( 클수록 축소되 보이는 미니맵 )
	DepthCaptureComp->OrthoWidth = 3072;
	DepthCaptureComp->CaptureSource = ESceneCaptureSource::SCS_SceneDepth;
	DepthCaptureComp->ShowFlags.SetSkeletalMeshes(false);
	DepthCaptureComp->ShowFlags.SetParticles(false);
	DepthCaptureComp->ShowFlags.SetCollisionPawn(false);
	DepthCaptureComp->ShowFlags.SetPostProcessing(true);   // 포스트 프로세싱 활성화 
	DepthCaptureComp->ShowFlags.SetSkyLighting(false);
	DepthCaptureComp->ShowFlags.SetWidgetComponents(false);
	
	//ConstructorHelpers::FObjectFinder<UTextureRenderTarget2D> MinimapRenderTarget2D(TEXT("/Game/Materials/T_MinimapRenderTarget2D"));
	////CHelpers::GetAsset(&MinimapRenderTarget2D, TEXT("/Game/Materials/T_MinimapRenderTarget2D"));
	//if (MinimapRenderTarget2D.Succeeded())
	//{
	//	SceneCaptureComp->TextureTarget = MinimapRenderTarget2D.Object;
	//}
}

void ACMinimapCameraActor::BeginPlay()
{
	Super::BeginPlay();

	CreateMiniMapRenderTarget();
}

void ACMinimapCameraActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SceneCaptureComp->CaptureScene();
}

void ACMinimapCameraActor::CreateMiniMapRenderTarget()
{
	// 렌더 타겟 생성
	MiniMapRenderTarget = NewObject<UTextureRenderTarget2D>(this);
	MiniMapRenderTarget->InitAutoFormat(512, 512);  // 텍스처 크기 설정

	// 씬 캡처 컴포넌트 설정
	SceneCaptureComp->TextureTarget = MiniMapRenderTarget;
	//SceneCaptureComp->CaptureSource = ESceneCaptureSource::SCS_FinalColorLDR;  // 렌더링 소스 설정

	SceneCaptureComp->CaptureScene();

	// 렌더 타겟 생성
	MiniMapDepthRenderTarget = NewObject<UTextureRenderTarget2D>(this);
	MiniMapDepthRenderTarget->InitAutoFormat(512, 512);  // 텍스처 크기 설정

	// 씬 캡처 컴포넌트 설정
	DepthCaptureComp->TextureTarget = MiniMapDepthRenderTarget;
	//DepthCaptureComp->CaptureSource = ESceneCaptureSource::SCS_FinalColorLDR;  // 렌더링 소스 설정
	DepthCaptureComp->CaptureScene();

}

