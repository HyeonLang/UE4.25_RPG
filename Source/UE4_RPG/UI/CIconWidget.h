#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CIconWidget.generated.h"

class ACPlayerCharacter;
class UCAction;
class UTexture2D;
class UImage;

UCLASS()
class UE4_RPG_API UCIconWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual bool Initialize() override;

protected:
	UFUNCTION(BlueprintCallable)
	void SetIcon(UTexture2D* Icon);

protected:
	UPROPERTY(meta = (BindWidget))
	UImage* IconImage;
};
