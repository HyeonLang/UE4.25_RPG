#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "CPlayerState.generated.h"


class UCInventory;

UCLASS()
class UE4_RPG_API ACPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	ACPlayerState();
public:
	bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;

	virtual void PostInitializeComponents() override;

	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE UCInventory* GetPlayerInventory() const { return PlayerInventory; }

protected:
	//UPROPERTY(VisibleDefaultsOnly)
	UPROPERTY(Replicated, VisibleDefaultsOnly)
	UCInventory* PlayerInventory;
	
};
