#pragma once

#include "CoreMinimal.h"
#include "../CPlayerCharacter.h"
#include "CPlayerCharacter_Gideon.generated.h"

UCLASS()
class UE4_RPG_API ACPlayerCharacter_Gideon : public ACPlayerCharacter
{
	GENERATED_BODY()

public:
	ACPlayerCharacter_Gideon();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

protected:
	
	
};
