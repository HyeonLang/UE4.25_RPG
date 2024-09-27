#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CWeapon.generated.h"

class ACharacter;

UCLASS()
class UE4_RPG_API ACWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	ACWeapon();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnEquip();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnUnequip();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ActorAttachTo(FName InSoketName);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ComponentAttachTo(USceneComponent* InComponent, FName InSoketName);

protected:
	UPROPERTY(VisibleDefaultsOnly)
	USceneComponent* RootComp;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	USkeletalMeshComponent* SkeletalMeshComp;

	UPROPERTY(BlueprintReadOnly)
	ACharacter* OwnerCharacter;

};
