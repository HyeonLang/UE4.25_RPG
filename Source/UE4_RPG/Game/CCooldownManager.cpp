// Fill out your copyright notice in the Description page of Project Settings.


#include "CCooldownManager.h"

#include "Net/UnrealNetwork.h"
#include "CCooldownManager.h"
#include "TimerManager.h"

#include "Actions/CAction.h"

UCCooldownManager::UCCooldownManager()
    : CurrentCooldown(0.0f)
    , RemainingCooldown(0.0f)
    , bIsCooldownActive(false)
{
}

void UCCooldownManager::CooldownTick(float DeltaTime)
{
    if (GetWorld() && GetWorld()->IsServer())
    { 
        if (bIsCooldownActive)
        {
            RemainingCooldown = GetWorld()->GetTimerManager().GetTimerRemaining(CooldownTimerHandle);
        }
    }
}

void UCCooldownManager::SetWorld(UWorld* NewWorld)
{
    World = NewWorld;
}

UWorld* UCCooldownManager::GetWorld() const
{
    UCAction* Action = Cast<UCAction>(GetOuter());
    if (Action)
    {
        return Action->GetWorld();
    }

    AActor* Actor = Cast<AActor>(GetOuter());
    if (Actor)
    {
        return Actor->GetWorld();
    }

    return nullptr;
}



void UCCooldownManager::StartCooldown(float InitialCooldown)
{
    if (InitialCooldown <= 0.0f)
    {
        UE_LOG(LogTemp, Warning, TEXT("Cooldown is set to 0 or below, ignoring StartCooldown."));
        return;
    }

    CurrentCooldown = InitialCooldown;
    RemainingCooldown = InitialCooldown;
    bIsCooldownActive = true;

    GetWorld()->GetTimerManager().SetTimer(
        CooldownTimerHandle,
        this,
        &UCCooldownManager::OnCooldownComplete,
        InitialCooldown,
        false
    );
}

bool UCCooldownManager::IsCooldownActive() const
{
    return bIsCooldownActive;
}

float UCCooldownManager::GetRemainingCooldown() const
{
    return RemainingCooldown;
}

void UCCooldownManager::AdjustCooldownBySeconds(float Seconds)
{
    if (!bIsCooldownActive)
    {
        UE_LOG(LogTemp, Warning, TEXT("No active cooldown to adjust."));
        return;
    }

    RemainingCooldown = FMath::Max(0.0f, GetRemainingCooldown() + Seconds);

    if (RemainingCooldown > 0.0f)
    {
        GetWorld()->GetTimerManager().SetTimer(
            CooldownTimerHandle,
            this,
            &UCCooldownManager::OnCooldownComplete,
            RemainingCooldown,
            false
        );
    }
    else
    {
        OnCooldownComplete();
    }
}

void UCCooldownManager::AdjustCooldownByMultiplier(float Multiplier)
{
    if (!bIsCooldownActive)
    {
        UE_LOG(LogTemp, Warning, TEXT("No active cooldown to adjust."));
        return;
    }

    RemainingCooldown = FMath::Max(0.0f, GetRemainingCooldown() * Multiplier);

    if (RemainingCooldown > 0.0f)
    {
        GetWorld()->GetTimerManager().SetTimer(
            CooldownTimerHandle,
            this,
            &UCCooldownManager::OnCooldownComplete,
            RemainingCooldown,
            false
        );
    }
    else
    {
        OnCooldownComplete();
    }
}

void UCCooldownManager::OnCooldownComplete()
{
    bIsCooldownActive = false;
    RemainingCooldown = 0.0f;

    GetWorld()->GetTimerManager().ClearTimer(CooldownTimerHandle);

    UE_LOG(LogTemp, Log, TEXT("Cooldown complete."));
}

void UCCooldownManager::OnRep_CurrentCooldown()
{
}

void UCCooldownManager::OnRep_RemainingCooldown()
{
}

void UCCooldownManager::OnRep_bIsCooldownActive()
{
}

void UCCooldownManager::SetCooldown(float CooldownTime)
{
    // ��Ÿ���� 0 ���϶�� �������� ����
    if (CooldownTime <= 0.0f)
    {
        UE_LOG(LogTemp, Warning, TEXT("Cooldown time must be greater than 0."));
        return;
    }

    // ���� Ÿ�̸Ӹ� �ʱ�ȭ
    GetWorld()->GetTimerManager().ClearTimer(CooldownTimerHandle);

    // ���ο� ��Ÿ�� ����
    CurrentCooldown = CooldownTime;
    RemainingCooldown = CooldownTime;
    bIsCooldownActive = true;

    // Ÿ�̸� ����
    GetWorld()->GetTimerManager().SetTimer(
        CooldownTimerHandle,
        this,
        &UCCooldownManager::OnCooldownComplete,
        CooldownTime,
        false
    );

    UE_LOG(LogTemp, Log, TEXT("Cooldown set to: %f seconds"), CooldownTime);
}


void UCCooldownManager::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UCCooldownManager, CurrentCooldown);
    DOREPLIFETIME(UCCooldownManager, RemainingCooldown);
    DOREPLIFETIME(UCCooldownManager, bIsCooldownActive);

}