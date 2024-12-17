// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CMinimapCameraActor.generated.h"

class USceneCaptureComponent2D;
class USpringArmComponent;
class ACPlayerController;
class UTextureRenderTarget2D;
class UPaperSpriteComponent;


UCLASS()
class UE4_RPG_API ACMinimapCameraActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACMinimapCameraActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


public:
	FORCEINLINE USceneCaptureComponent2D* GetSceneCapture2DComp() const { return SceneCaptureComp; }
	FORCEINLINE USpringArmComponent* GetSpringArmComp() const { return SpringArmComp; }

	void CreateMiniMapRenderTarget();

protected:
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Components")
	USceneCaptureComponent2D* SceneCaptureComp;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Components")
	USceneCaptureComponent2D* DepthCaptureComp;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Components")
	UPaperSpriteComponent* PlayerMarkerSpriteComp;


	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Components")
	USpringArmComponent* SpringArmComp;

	USceneComponent* SceneComp;

	UTextureRenderTarget2D* MiniMapRenderTarget;
	UTextureRenderTarget2D* MiniMapDepthRenderTarget;

};
