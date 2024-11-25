// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "CWorldWidgetComponent.generated.h"

/**
 * 
 */
UCLASS()
class UE4_RPG_API UCWorldWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
public:
	UCWorldWidgetComponent();

public:
	virtual void InitWidget() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

};
