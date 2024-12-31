#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CActorSpawner.generated.h"

class UCCooldownManager;

UCLASS()
class UE4_RPG_API ACActorSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	ACActorSpawner();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UPROPERTY(BlueprintReadOnly, Replicated, Category = "Cooldown")
	UCCooldownManager* SpawnCooldownManager;

};
