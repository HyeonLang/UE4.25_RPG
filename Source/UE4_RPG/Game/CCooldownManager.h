#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CCooldownManager.generated.h"


UCLASS(BlueprintType)
class UE4_RPG_API UCCooldownManager : public UObject
{
	GENERATED_BODY()

public:
    // 쿨타임 초기화
    UCCooldownManager();

    // Object류 클래스 리플리케이트 준비작업
    // 액터가 아닌 오브젝트상속 클래스의 리플리케이트를 도와줌
    FORCEINLINE virtual bool IsSupportedForNetworking() const override
    {
        return true;
    }

public:
    void CooldownTick(float DeltaTime);

    void SetWorld(UWorld* NewWorld);

    UWorld* GetWorld() const override;

    /** 쿨타임 시작 함수 */
    UFUNCTION(BlueprintCallable, Category = "Cooldown")
    void StartCooldown(float InitialCooldown);

    /** 쿨타임이 진행 중인지 확인 */
    UFUNCTION(BlueprintCallable, Category = "Cooldown")
    bool IsCooldownActive() const;

    UFUNCTION(BlueprintCallable, Category = "Cooldown")
    float GetRemainingCooldown() const;

    /** 쿨타임을 특정 시간만큼 증가 또는 감소 */
    UFUNCTION(BlueprintCallable, Category = "Cooldown")
    void AdjustCooldownBySeconds(float Seconds);

    /** 쿨타임을 특정 비율로 증가 또는 감소 */
    UFUNCTION(BlueprintCallable, Category = "Cooldown")
    void AdjustCooldownByMultiplier(float Multiplier);

    /** 쿨타임을 특정 값으로 설정 */
    UFUNCTION(BlueprintCallable, Category = "Cooldown")
    void SetCooldown(float CooldownTime);

    
    UFUNCTION(BlueprintCallable, Category = "Cooldown")
    void SetLocalMode();


protected:
    /** 쿨타임 종료 시 호출 */
    void OnCooldownComplete();

    /** 타이머 핸들 */
    FTimerHandle CooldownTimerHandle;

    /** 현재 쿨타임 값 */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = "OnRep_CurrentCooldown")
    float CurrentCooldown;

    UFUNCTION()
    void OnRep_CurrentCooldown();

    /** 남은 쿨타임 */
    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = "OnRep_RemainingCooldown")
    float RemainingCooldown;

    UFUNCTION()
    void OnRep_RemainingCooldown();

    /** 쿨타임 활성 상태 */
    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = "OnRep_bIsCooldownActive")
    bool bIsCooldownActive;

    UFUNCTION()
    void OnRep_bIsCooldownActive();

    UPROPERTY()
    UWorld* World;

    UPROPERTY()
    bool bIsLocalMode;
};
