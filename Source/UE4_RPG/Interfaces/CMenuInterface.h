#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CMenuInterface.generated.h"

UINTERFACE(MinimalAPI)
class UCMenuInterface : public UInterface
{
	GENERATED_BODY()
};

class UE4_RPG_API ICMenuInterface
{
	GENERATED_BODY()

public:
	virtual void Login(const FString& UserId, const FString& Password) = 0;
	virtual void Host(FString InDesiredSessionName) = 0;
	virtual void Join(uint32 InIndex) = 0;
	virtual void OpenLoginMenuLevel() = 0;
	virtual void StartFindSession() = 0;
};
