#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "CNPCActionComponent.generated.h"

class UCNPCAction;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnNPCActionStateChanged, UCNPCActionComponent*, OwningComp, UCNPCAction*, Action);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNPCActionCreateFinished, UCNPCActionComponent*, OwningComp);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE4_RPG_API UCNPCActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCNPCActionComponent();

protected:
	virtual void InitializeComponent() override;
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Object류 클래스 리플리케이트 준비작업 1 재정의
	bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;

public:
	UFUNCTION(BlueprintCallable, Category = "Action")
	void AddNPCAction(AActor* Instigator, TSubclassOf<UCNPCAction> NPCActionClass);

	UFUNCTION(BlueprintCallable, Category = "Action")
	UCNPCAction* GetNPCAction(TSubclassOf<UCNPCAction> NPCActionClass) const;

	UFUNCTION(BlueprintCallable, Category = "Action")
	UCNPCAction* GetNPCActionByName(FName InNPCActionName) const;

	UFUNCTION(BlueprintCallable, Category = "Action")
	void RemoveNPCAction(UCNPCAction* NPCActionToRemove);

	UFUNCTION(BlueprintCallable, Category = "Action")
	bool StartNPCActionByName(AActor* Instigator, FName NPCActionName);

	UFUNCTION(BlueprintCallable, Category = "Action")
	bool StopNPCActionByName(AActor* Instigator, FName NPCActionName);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Action")
	FORCEINLINE TArray<UCNPCAction*> GetNPCActions() const { return NPCActions; }

protected:
	UFUNCTION(Reliable, Server)
	void ServerStartNPCAction(AActor* Instigator, FName NPCActionName);

	UFUNCTION(Reliable, Server)
	void ServerStopNPCAction(AActor* Instigator, FName NPCActionName);

public:
	UPROPERTY(BlueprintAssignable)
	FOnNPCActionCreateFinished OnNPCActionCreateFinished;

	UPROPERTY(BlueprintReadWrite, Category = "Montage")
	bool bCanStopMontagePostNPCAction;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GameplayTag")
	FGameplayTagContainer ActiveGameplayTags;

	UPROPERTY(BlueprintAssignable)
	FOnNPCActionStateChanged OnNPCActionStarted;

	UPROPERTY(BlueprintAssignable)
	FOnNPCActionStateChanged OnNPCActionStopped;

	UPROPERTY(BlueprintReadWrite)
	UCNPCAction* ActiveMontageNPCAction;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	TArray<TSubclassOf<UCNPCAction>> DefaultNPCActions;

	UPROPERTY(BlueprintReadOnly, Replicated)
	TArray<UCNPCAction*> NPCActions;
};
