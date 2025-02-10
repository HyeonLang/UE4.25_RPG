
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CScreenWidget.generated.h"

class USizeBox;
class UTextBlock;


UCLASS()
class UE4_RPG_API UCScreenWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	void SetText(FText Text);

public:
	UPROPERTY(BlueprintReadOnly, Category = "UI", meta = (ExposeOnSpawn = true))
	FVector AttachToLocation;

	UPROPERTY(BlueprintReadWrite, Category = "UI", meta = (ExposeOnSpawn = true))
	FText InText;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI")
	FVector WorldOffset;

protected:
	UPROPERTY(meta = (BindWidget))
	USizeBox* ParentSizeBox;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* DamageText;
};
