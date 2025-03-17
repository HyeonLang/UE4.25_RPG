// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/CInteractionInterface.h"
#include "CChest.generated.h"

class UBoxComponent;
class UParticleSystemComponent;

UCLASS()
class UE4_RPG_API ACChest : public AActor, public ICInteractionInterface
{
	GENERATED_BODY()
	
public:	
	ACChest();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
	virtual FName GetInteractName_Implementation(APawn* InstigatorPawn) override;

	

protected:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetCanOpen(bool bInNew);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SpawnItems(APawn* InstigatorPawn);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void DestroyAction();

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float MaxPitch;


protected:
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Components")
	UBoxComponent* BoxComp;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Components")
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Components")
	UStaticMeshComponent* LidMesh;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Components")
	UStaticMeshComponent* TreasureMesh;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Components")
	UParticleSystemComponent* EffectComp;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, ReplicatedUsing = "OnRep_CanOpen")
	bool bCanOpen;

	UFUNCTION()
	void OnRep_CanOpen();

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = "OnRep_LidOpen")
	bool bLidOpen;

	UFUNCTION()
	void OnRep_LidOpen();

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TArray<FName> SpawnItemID;
};
