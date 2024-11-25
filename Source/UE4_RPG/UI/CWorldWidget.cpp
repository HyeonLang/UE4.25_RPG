#include "CWorldWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/SizeBox.h"
#include "Components/WidgetComponent.h"
#include "Global.h"

bool UCWorldWidget::Initialize()
{
    bool result = Super::Initialize();
    return result;
}

void UCWorldWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);


}

void UCWorldWidget::SetBindDelegate_Implementation()
{
}
