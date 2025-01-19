// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CSessionRowWidget.generated.h"

class UTextBlock;
class UCLoginMenuWidget;
class UButton;

UCLASS()
class UE4_RPG_API UCSessionRowWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void Setup(UCLoginMenuWidget* InParentWidget, uint32 InSelfIndex);

public:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* SessionName;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* HostUser;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ConnectionFraction;

private:
	UFUNCTION()
	void OnClicked();

public:
	UPROPERTY(BlueprintReadOnly)
	bool bEverClicked;

private:
	UPROPERTY(meta = (BindWidget))
	UButton* RowButton;



private:
	UCLoginMenuWidget* ParentWidget;
	uint32 SelfIndex;
};
