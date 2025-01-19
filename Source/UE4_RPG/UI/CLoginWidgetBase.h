// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/CMenuInterface.h"
#include "CLoginWidgetBase.generated.h"



UCLASS()
class UE4_RPG_API UCLoginWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetOwningInstance(ICMenuInterface* InOwningInstance);
	void SetInputToUI();
	void SetInputToGame();

protected:
	ICMenuInterface* OwningInstance;

};
