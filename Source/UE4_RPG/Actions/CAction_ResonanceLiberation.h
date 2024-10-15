#pragma once

#include "CoreMinimal.h"
#include "Actions/CAction.h"
#include "CAction_ResonanceLiberation.generated.h"

class ACPlayerCameraActor;

UCLASS()
class UE4_RPG_API UCAction_ResonanceLiberation : public UCAction
{
	GENERATED_BODY()

public:
	UCAction_ResonanceLiberation();

	virtual void ActionTick_Implementation(float DeltaTime) override;
	virtual void SetActionDatas() override;

public:
	virtual void StartAction_Implementation(AActor* Instigator) override;
	virtual void StopAction_Implementation(AActor* Instigator) override;

public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE ACPlayerCameraActor* GetLiberationCameraActor() const { return LiberationCameraActor; }
	FORCEINLINE FVector GetPlayingCameraOffset() const { return PlayingCameraOffset; }
	void SetPlayingCameraOffset(FVector NewOffset);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Attack_Elapsed(ACharacter* InstigatorCharacter);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void PlayCameraAction(ACPlayerCharacter* InstigatorCharacter);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void EndCameraAction(ACPlayerCharacter* InstigatorCharacter);


protected:
	UPROPERTY(BlueprintReadWrite, Category = "Target")
	AActor* TargetActor;
	UPROPERTY(BlueprintReadWrite, Category = "Target")
	FRotator TargetDerection;

private:
	UPROPERTY(Replicated)
	ACPlayerCameraActor* LiberationCameraActor;

	FVector PlayingCameraOffset;
	
};
