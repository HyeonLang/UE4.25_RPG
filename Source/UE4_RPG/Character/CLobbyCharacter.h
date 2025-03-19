// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CLobbyCharacter.generated.h"

class UTextRenderComponent;

UCLASS()
class UE4_RPG_API ACLobbyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACLobbyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE FText GetUserName() const { return UserName; }

	UFUNCTION(BlueprintCallable)
	void SetUserName(FText InName);
protected:
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	UTextRenderComponent* UserNameComp;

	UPROPERTY(BlueprintReadWrite, ReplicatedUsing = "Rep_UserName")
	FText UserName;

	UFUNCTION()
	void Rep_UserName();
};
