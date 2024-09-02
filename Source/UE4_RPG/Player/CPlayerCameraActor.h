#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPlayerCameraActor.generated.h"

class USceneComponent;
class UCameraComponent;
class USpringArmComponent;

UCLASS()
class UE4_RPG_API ACPlayerCameraActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ACPlayerCameraActor();

protected:
	virtual void BeginPlay() override;

public:
	FORCEINLINE const UCameraComponent* GetCameraComp() { return CameraComp; }
	FORCEINLINE const USpringArmComponent* GetSpringArmComp() { return SpringArmComp; }

	void OnTurn();
	void OnLookUp();

protected:
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Components")
	UCameraComponent* CameraComp;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Components")
	USpringArmComponent* SpringArmComp;

	USceneComponent* SceneComp;

};
