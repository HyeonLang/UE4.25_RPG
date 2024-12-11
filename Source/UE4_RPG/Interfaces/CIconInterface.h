

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CIconInterface.generated.h"

class UTexture2D;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCIconInterface : public UInterface
{
	GENERATED_BODY()
};


class UE4_RPG_API ICIconInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	TSoftObjectPtr<UTexture2D> GetIcon() const;
	
};
