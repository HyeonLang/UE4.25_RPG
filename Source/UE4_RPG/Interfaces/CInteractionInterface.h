// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CInteractionInterface.generated.h"


UINTERFACE(MinimalAPI)
class UCInteractionInterface : public UInterface
{
	GENERATED_BODY()
};


class UE4_RPG_API ICInteractionInterface
{
	GENERATED_BODY()


public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Interact(APawn* InstigatorPawn);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FName GetInteractName(APawn* InstigatorPawn);
};
