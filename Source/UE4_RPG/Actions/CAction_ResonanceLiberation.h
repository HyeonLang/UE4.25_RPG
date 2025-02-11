#pragma once

#include "CoreMinimal.h"
#include "Actions/CAction.h"
#include "CAction_ResonanceLiberation.generated.h"

class ACPlayerCineCameraActor;


UCLASS()
class UE4_RPG_API UCAction_ResonanceLiberation : public UCAction
{
	GENERATED_BODY()

public:
	UCAction_ResonanceLiberation();
	virtual void Initialize() override;

	virtual void ActionTick_Implementation(float DeltaTime) override;
	virtual void SetActionDatas_Implementation() override;

public:
	virtual void StartAction_Implementation(AActor* Instigator) override;
	virtual void StopAction_Implementation(AActor* Instigator) override;

public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE ACPlayerCineCameraActor* GetLiberationCameraActor() const { return LiberationCameraActor; }
	FORCEINLINE FVector GetPlayingCameraOffset() const { return PlayingCameraOffset; }
	void SetPlayingCameraOffset(FVector NewOffset);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void StartCameraAction(ACPlayerCharacter* InstigatorCharacter);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void PlayCameraAction(ACPlayerCharacter* InstigatorCharacter);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void EndCameraAction(ACPlayerCharacter* InstigatorCharacter);

protected:
	UFUNCTION()
	void OnPlayCinematicFinished(ACPlayerCineCameraActor* CineCamera);


protected:
	UPROPERTY(BlueprintReadWrite, Category = "Target", ReplicatedUsing = "OnRep_Target")
	AActor* TargetActor;

	UFUNCTION()
	void OnRep_Target();

	UPROPERTY(BlueprintReadWrite, Category = "Target", ReplicatedUsing = "OnRep_TargetDerection")
	FVector TargetLocation;

	UPROPERTY(BlueprintReadWrite, Category = "Target", ReplicatedUsing = "OnRep_TargetDerection")
	FRotator TargetDerection;

	UFUNCTION()
	void OnRep_TargetLocation();

	UFUNCTION()
	void OnRep_TargetDerection();

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ACPlayerCineCameraActor> LiberationCameraActorClass;

	UPROPERTY(BlueprintReadOnly)
	ACPlayerCineCameraActor* LiberationCameraActor;

	FVector PlayingCameraOffset;

	UPROPERTY(EditDefaultsOnly)
	float LiberationCameraOutBlendTime;


	
};
