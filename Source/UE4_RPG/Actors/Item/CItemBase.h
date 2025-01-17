#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/CInteractionInterface.h"
#include "CItemBase.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UParticleSystem;
class UParticleSystemComponent;

UCLASS()
class UE4_RPG_API ACItemBase : public AActor, public ICInteractionInterface
{
	GENERATED_BODY()
	
public:	
	ACItemBase();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
	virtual FName GetInteractName_Implementation(APawn* InstigatorPawn) override;

protected:
	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	USphereComponent* SphereComp;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Components")
	UParticleSystemComponent* EffectComp;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Info")
	FName ItemID;
};
