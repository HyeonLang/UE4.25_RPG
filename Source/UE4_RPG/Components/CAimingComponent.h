#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CAimingComponent.generated.h"

#define BOSS_BASESCORE 9999.f;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE4_RPG_API UCAimingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCAimingComponent();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetDistanceWeight() const { return DistanceWeight; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetCameraDirectionWeight() const { return CameraDirectionWeight; }

	UFUNCTION(BlueprintCallable)
	void SetDistanceWeight(float InValue);
	UFUNCTION(BlueprintCallable)
	void SetCameraDirectionWeight(float InValue);

public:
	UFUNCTION(BlueprintCallable, Category = "Aiming")
	void GetAimTargetDirection(FRotator& OutDirection, AActor* OutTarget, const float InRange, const bool InIsBossMode);

	//UFUNCTION(BlueprintCallable, Category = "Aiming");

private:
	float CalcWeight(float Dot, float Distance, float InRange);

private:
	UPROPERTY(EditAnywhere, Category = "Weight")
	float DistanceWeight;
	UPROPERTY(EditAnywhere, Category = "Weight")
	float CameraDirectionWeight;

	UPROPERTY(EditDefaultsOnly, Category = "Weight")
	float DotBaseScore;

	UPROPERTY(EditDefaultsOnly, Category = "Weight")
	float DistanceBaseScore;

public:
	AActor* TargetActor;
		
};
