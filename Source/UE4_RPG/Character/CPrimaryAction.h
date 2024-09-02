
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPrimaryAction.generated.h"

UCLASS()
class UE4_RPG_API ACPrimaryAction : public AActor
{
	GENERATED_BODY()
	
public:	
	ACPrimaryAction();

protected:
	virtual void BeginPlay() override;

public:
	void EnableCombo();
	void DisableCombo();

private:
	int32 ComboCount;
	bool bCanCombo;
	bool bSuccessCombo;

};
