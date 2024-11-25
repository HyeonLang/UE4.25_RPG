// Fill out your copyright notice in the Description page of Project Settings.


#include "CWorldWidgetComponent.h"
#include "Blueprint/UserWidget.h"
#include "Global.h"
#include "UI/CWorldWidget.h"

UCWorldWidgetComponent::UCWorldWidgetComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UCWorldWidgetComponent::InitWidget()
{
    Super::InitWidget();

    if (Widget)
    {
        UCWorldWidget* WorldWidget = Cast<UCWorldWidget>(Widget);
        if (WorldWidget)
        {
            WorldWidget->ParentWidgetComponent = this;
            WorldWidget->SetBindDelegate();
        }
    }
}

void UCWorldWidgetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // Local Player의 PlayerController 가져오기
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (PlayerController)
    {
        // PlayerController의 ViewTarget(현재 활성화된 카메라) 반환
        
        FVector CameraLocation;
        FRotator CameraRotation;
        PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);

        // 위젯 컴포넌트가 카메라를 바라보도록 설정
        FRotator LookAtRotation = (CameraLocation - GetComponentLocation()).Rotation();
        SetWorldRotation(LookAtRotation);
    }
}

