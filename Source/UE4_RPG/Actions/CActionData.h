#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CActionData.generated.h"

class ACDoAction;
class UAnimMontage;
class UParticleSystem;


USTRUCT(BlueprintType)
struct FMontageData
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
};

USTRUCT(BlueprintType)
struct FActionData : public FMontageData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	float Power = 1.f;

	UPROPERTY(EditDefaultsOnly)
	FTransform EffectTransform;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UParticleSystem> Effects;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCameraShake> ShakeClass;

};

UCLASS()
class UE4_RPG_API UCActionData : public UDataAsset
{
	GENERATED_BODY()

public:
	void BeginPlay(ACharacter* InOwnerCharacter);
	

public:
	UPROPERTY(EditAnywhere, Category = "DoAction")
	TArray<FActionData> ActionDatas;
};
