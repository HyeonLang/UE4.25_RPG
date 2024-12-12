#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CWeapon.generated.h"

class ACharacter;
class UCapsuleComponent;
class UCAction;

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

public:
	UFUNCTION(BlueprintCallable)
	void OnCollision(UCAction* NewAction = nullptr, int32 NewAttackIndex = 0);

	UFUNCTION(BlueprintCallable)
	void OffCollision();

	UFUNCTION(BlueprintCallable)
	bool IsOverlappedActor(AActor* TargetActor);

protected:
	UFUNCTION()
	void OnActorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnActorEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


public:
	UPROPERTY(VisibleDefaultsOnly)
	USceneComponent* MidComp;
	UPROPERTY(VisibleDefaultsOnly)
	USceneComponent* StartComp;
	UPROPERTY(BlueprintReadWrite)
	TArray<AActor*> OverlappedActors;
	UPROPERTY(BlueprintReadWrite)
	UCAction* InstigateAction;
	UPROPERTY(BlueprintReadWrite)
	int32 AttackIndex;

protected:
	UPROPERTY(VisibleDefaultsOnly)
	USceneComponent* RootComp;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	USkeletalMeshComponent* SkeletalMeshComp;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	UCapsuleComponent* CapsuleComp;

	UPROPERTY(BlueprintReadOnly)
	ACharacter* OwnerCharacter;

};
