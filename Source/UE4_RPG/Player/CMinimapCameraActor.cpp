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

	//ī�޶� ���� Ÿ���� ������ ��ȯ�Ͽ� �Ÿ����� ���� �̹����� ����
	SceneCaptureComp->ProjectionType = ECameraProjectionMode::Orthographic;
	//ī�޶󿡼� ĸó�� ũ�� ( Ŭ���� ��ҵ� ���̴� �̴ϸ� )
	SceneCaptureComp->OrthoWidth = 3072;

	SceneCaptureComp->CaptureSource = ESceneCaptureSource::SCS_SceneColorSceneDepth;
	SceneCaptureComp->ShowFlags.SetSkeletalMeshes(false);
	SceneCaptureComp->ShowFlags.SetParticles(false);
	SceneCaptureComp->ShowFlags.SetDynamicShadows(false);
	SceneCaptureComp->ShowFlags.SetCollisionPawn(false);
	SceneCaptureComp->ShowFlags.SetPostProcessing(true);   // ����Ʈ ���μ��� Ȱ��ȭ 
	SceneCaptureComp->ShowFlags.SetSkyLighting(false);
	SceneCaptureComp->ShowFlags.SetWidgetComponents(false);
	SceneCaptureComp->ShowFlags.SetSingleFlag(FEngineShowFlags::EShowFlag::SF_ContactShadows, false);

	//ī�޶� ���� Ÿ���� ������ ��ȯ�Ͽ� �Ÿ����� ���� �̹����� ����
	DepthCaptureComp->ProjectionType = ECameraProjectionMode::Orthographic;
	//ī�޶󿡼� ĸó�� ũ�� ( Ŭ���� ��ҵ� ���̴� �̴ϸ� )
	DepthCaptureComp->OrthoWidth = 3072;
	DepthCaptureComp->CaptureSource = ESceneCaptureSource::SCS_SceneDepth;
	DepthCaptureComp->ShowFlags.SetSkeletalMeshes(false);
	DepthCaptureComp->ShowFlags.SetParticles(false);
	DepthCaptureComp->ShowFlags.SetCollisionPawn(false);
	DepthCaptureComp->ShowFlags.SetPostProcessing(true);   // ����Ʈ ���μ��� Ȱ��ȭ 
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
	// ���� Ÿ�� ����
	MiniMapRenderTarget = NewObject<UTextureRenderTarget2D>(this);
	MiniMapRenderTarget->InitAutoFormat(512, 512);  // �ؽ�ó ũ�� ����

	// �� ĸó ������Ʈ ����
	SceneCaptureComp->TextureTarget = MiniMapRenderTarget;
	//SceneCaptureComp->CaptureSource = ESceneCaptureSource::SCS_FinalColorLDR;  // ������ �ҽ� ����

	SceneCaptureComp->CaptureScene();

	// ���� Ÿ�� ����
	MiniMapDepthRenderTarget = NewObject<UTextureRenderTarget2D>(this);
	MiniMapDepthRenderTarget->InitAutoFormat(512, 512);  // �ؽ�ó ũ�� ����

	// �� ĸó ������Ʈ ����
	DepthCaptureComp->TextureTarget = MiniMapDepthRenderTarget;
	//DepthCaptureComp->CaptureSource = ESceneCaptureSource::SCS_FinalColorLDR;  // ������ �ҽ� ����
	DepthCaptureComp->CaptureScene();

}

