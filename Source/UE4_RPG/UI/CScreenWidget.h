
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CScreenWidget.generated.h"

class USizeBox;


UCLASS()
class UE4_RPG_API UCScreenWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	UPROPERTY(BlueprintReadOnly, Category = "UI", meta = (ExposeOnSpawn = true))
	AActor* AttachToActor;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI")
	FVector WorldOffset;

protected:
	UPROPERTY(meta = (BindWidget))
	USizeBox* ParentSizeBox;
	
};
