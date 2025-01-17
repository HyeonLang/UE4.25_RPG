// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CDelegateWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnButtonClicked);

class UTextBlock;

UCLASS()
class UE4_RPG_API UCDelegateWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual bool Initialize() override;

public:
	UFUNCTION(BlueprintCallable)
	void BroadcastCancelButtonClicked();

	UFUNCTION(BlueprintCallable)
	void BroadcastOKButtonClicked();

	UFUNCTION(BlueprintCallable)
	void ClearAllDelegates();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void CreateWidget();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void RemoveWidget();

public:
	UPROPERTY(BlueprintAssignable)
	FOnButtonClicked OnCancelButtonClicked;

	UPROPERTY(BlueprintAssignable)
	FOnButtonClicked OnOKButtonClicked;

protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* DescriptionText;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true))
	FText Description;

};
