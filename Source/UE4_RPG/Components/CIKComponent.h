#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CIKComponent.generated.h"

class ACharacter;
class ACPlayerCharacter;

USTRUCT(BlueprintType)
struct FIKFeetData
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FVector LeftDistance;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FVector RightDistance;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FVector PelvisDistance;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FRotator LeftRotation;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FRotator RightRotation;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE4_RPG_API UCIKComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCIKComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE FIKFeetData GetIKFeetData() { return IKFeetData; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool IsIKMode() { return bIKMode; }

	UFUNCTION(BlueprintCallable)
	void SetIKMode(bool NewIkMode);

protected:
	UFUNCTION(BlueprintNativeEvent)
	void IKTrace(FName InSocketName, float& OutDistance, FRotator& OutRotation);

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "InverseKinemetics")
	FName LeftFootSocketName;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "InverseKinemetics")
	FName RightFootSocketName;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "InverseKinemetics")
	TEnumAsByte<EDrawDebugTrace::Type> DrawDebugType;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "InverseKinemetics")
	float Additional;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "InverseKinemetics")
	float FootHeight;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "InverseKinemetics")
	float InterpSpeed;

private:
	FIKFeetData IKFeetData;

	ACharacter* OwnerCharacter;
	ACPlayerCharacter* OwnerPlayerCharacter;

	float CapsuleHalfHeight;

	bool bIKMode;
};
