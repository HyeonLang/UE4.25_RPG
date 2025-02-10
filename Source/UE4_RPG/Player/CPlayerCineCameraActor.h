#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPlayerCineCameraActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayCinematicFinished, ACPlayerCineCameraActor*, CineCamera);

class USceneComponent;
class UCineCameraComponent;

UCLASS()
class UE4_RPG_API ACPlayerCineCameraActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ACPlayerCineCameraActor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void PlayCinematic();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void PlayCinematicFinished();

public:
	FOnPlayCinematicFinished OnPlayCinematicFinished;

protected:
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Components")
	UCineCameraComponent* CineCameraComp;

	USceneComponent* SceneComp;

};
