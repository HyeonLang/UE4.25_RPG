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
	virtual void PostInitializeComponents() override;

protected:
	UPROPERTY(Replicated)
	UCInventory* PlayerInventory;
	
};
