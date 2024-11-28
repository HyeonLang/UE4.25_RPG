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
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UAnimMontage* AnimMontage;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float PlayRate = 1.f;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FName StartSection;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	bool bCanMove = true;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	bool bLookForward = false;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	bool bCanStopMontagePostAction = false;

};

USTRUCT(BlueprintType)
struct FActionEffectData
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FTransform EffectTransform;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSubclassOf<UParticleSystem> Effects;

};

USTRUCT(BlueprintType)
struct FActionCameraData
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSubclassOf<UCameraShake> ShakeClass;
};


USTRUCT(BlueprintType)
struct FActionData
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TArray<FActionMontageData> MontageDatas;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TArray<FActionEffectData> EffectDatas;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TArray<FActionCameraData> CameraDatas;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float AttackRange;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float DamageRange;
};

UCLASS()
class UE4_RPG_API UCActionData : public UDataAsset
{
	GENERATED_BODY()

public:
	void BeginPlay(UCAction* InOwnerAction, TArray<FActionData>& OutActionDatas);
	
public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Actions")
	TArray<FActionData> ActionDatas;
};
