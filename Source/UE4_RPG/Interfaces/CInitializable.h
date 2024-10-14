#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CInitializable.generated.h"

UINTERFACE(MinimalAPI)
class UCInitializable : public UInterface
{
	GENERATED_BODY()
};

class UE4_RPG_API ICInitializable
{
	GENERATED_BODY()

public:
	virtual void Initialize() = 0;
};
