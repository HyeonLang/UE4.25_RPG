#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CWorldWidget.generated.h"

class USizeBox;
class UWidgetComponent;

UCLASS()
class UE4_RPG_API UCWorldWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual bool Initialize() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Beginplay")
	void SetBindDelegate();

public:
	UPROPERTY(BlueprintReadOnly, Category = "UI", meta = (ExposeOnSpawn = true))
	AActor* AttachToActor;

	UPROPERTY(BlueprintReadOnly, Category = "UI")
	UWidgetComponent* ParentWidgetComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI")
	FVector WorldOffset;
};
