#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CActionData.generated.h"

class UAnimMontage;
class UParticleSystem;
class ACPlayerChracter;
class UCAction;

USTRUCT(BlueprintType)
struct FActionMontageData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* AnimMontage;

	UPROPERTY(EditDefaultsOnly)
	float PlayRate = 1.f;

	UPROPERTY(EditDefaultsOnly)
	FName StartSection;

	UPROPERTY(EditDefaultsOnly)
	bool bCanMove = true;

	UPROPERTY(EditDefaultsOnly)
	bool bLookForward = false;

	UPROPERTY(EditDefaultsOnly)
	bool bCanStopMontagePostAction = false;

};

USTRUCT(BlueprintType)
struct FActionEffectData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	FTransform EffectTransform;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UParticleSystem> Effects;

};

USTRUCT(BlueprintType)
struct FActionCameraData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCameraShake> ShakeClass;
};


USTRUCT(BlueprintType)
struct FActionData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TArray<FActionMontageData> MontageDatas;

	UPROPERTY(EditDefaultsOnly)
	TArray<FActionEffectData> EffectDatas;

	UPROPERTY(EditDefaultsOnly)
	TArray<FActionCameraData> CameraDatas;

	UPROPERTY(EditAnywhere)
	float AttackRange;
};

UCLASS()
class UE4_RPG_API UCActionData : public UDataAsset
{
	GENERATED_BODY()

public:
	void BeginPlay(UCAction* InOwnerAction, TArray<FActionData>& OutActionDatas);
	
public:
	UPROPERTY(EditAnywhere, Category = "Actions")
	TArray<FActionData> ActionDatas;
};
