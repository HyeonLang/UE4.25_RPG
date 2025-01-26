#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CCooldownManager.generated.h"


UCLASS(BlueprintType)
class UE4_RPG_API UCCooldownManager : public UObject
{
	GENERATED_BODY()

public:
    // ��Ÿ�� �ʱ�ȭ
    UCCooldownManager();

    // Object�� Ŭ���� ���ø�����Ʈ �غ��۾�
    // ���Ͱ� �ƴ� ������Ʈ��� Ŭ������ ���ø�����Ʈ�� ������
    FORCEINLINE virtual bool IsSupportedForNetworking() const override
    {
        return true;
    }

public:
    void CooldownTick(float DeltaTime);

    void SetWorld(UWorld* NewWorld);

    UWorld* GetWorld() const override;

    /** ��Ÿ�� ���� �Լ� */
    UFUNCTION(BlueprintCallable, Category = "Cooldown")
    void StartCooldown(float InitialCooldown);

    /** ��Ÿ���� ���� ������ Ȯ�� */
    UFUNCTION(BlueprintCallable, Category = "Cooldown")
    bool IsCooldownActive() const;

    UFUNCTION(BlueprintCallable, Category = "Cooldown")
    float GetRemainingCooldown() const;

    /** ��Ÿ���� Ư�� �ð���ŭ ���� �Ǵ� ���� */
    UFUNCTION(BlueprintCallable, Category = "Cooldown")
    void AdjustCooldownBySeconds(float Seconds);

    /** ��Ÿ���� Ư�� ������ ���� �Ǵ� ���� */
    UFUNCTION(BlueprintCallable, Category = "Cooldown")
    void AdjustCooldownByMultiplier(float Multiplier);

    /** ��Ÿ���� Ư�� ������ ���� */
    UFUNCTION(BlueprintCallable, Category = "Cooldown")
    void SetCooldown(float CooldownTime);

    
    UFUNCTION(BlueprintCallable, Category = "Cooldown")
    void SetLocalMode();


protected:
    /** ��Ÿ�� ���� �� ȣ�� */
    void OnCooldownComplete();

    /** Ÿ�̸� �ڵ� */
    FTimerHandle CooldownTimerHandle;

    /** ���� ��Ÿ�� �� */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = "OnRep_CurrentCooldown")
    float CurrentCooldown;

    UFUNCTION()
    void OnRep_CurrentCooldown();

    /** ���� ��Ÿ�� */
    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = "OnRep_RemainingCooldown")
    float RemainingCooldown;

    UFUNCTION()
    void OnRep_RemainingCooldown();

    /** ��Ÿ�� Ȱ�� ���� */
    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = "OnRep_bIsCooldownActive")
    bool bIsCooldownActive;

    UFUNCTION()
    void OnRep_bIsCooldownActive();

    UPROPERTY()
    UWorld* World;

    UPROPERTY()
    bool bIsLocalMode;
};
