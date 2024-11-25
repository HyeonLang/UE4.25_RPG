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

    // Local Player�� PlayerController ��������
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (PlayerController)
    {
        // PlayerController�� ViewTarget(���� Ȱ��ȭ�� ī�޶�) ��ȯ
        
        FVector CameraLocation;
        FRotator CameraRotation;
        PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);

        // ���� ������Ʈ�� ī�޶� �ٶ󺸵��� ����
        FRotator LookAtRotation = (CameraLocation - GetComponentLocation()).Rotation();
        SetWorldRotation(LookAtRotation);
    }
}

