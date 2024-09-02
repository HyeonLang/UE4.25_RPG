// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../CPrimaryAction.h"
#include "CPrimaryAction_Gideon.generated.h"

UCLASS()
class UE4_RPG_API ACPrimaryAction_Gideon : public ACPrimaryAction
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACPrimaryAction_Gideon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
