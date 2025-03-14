#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "CActionComponent.generated.h"

class UCAction;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnActionStateChanged, UCActionComponent*, OwningComp, UCAction*, Action);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActionCreateFinished, UCActionComponent*, OwningComp);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE4_RPG_API UCActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCActionComponent();

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
	void AddAction(AActor* Instigator, TSubclassOf<UCAction> ActionClass);

	UFUNCTION(BlueprintCallable, Category = "Action")
	UCAction* GetAction(TSubclassOf<UCAction> ActionClass) const;

	UFUNCTION(BlueprintCallable, Category = "Action")
	UCAction* GetActionByName(FName InActionName) const;

	UFUNCTION(BlueprintCallable, Category = "Action")
	void RemoveAction(UCAction* ActionToRemove);

	UFUNCTION(BlueprintCallable, Category = "Action")
	bool StartActionByName(AActor* Instigator, FName ActionName, bool bBaseAction = true);

	UFUNCTION(BlueprintCallable, Category = "Action")
	bool StopActionByName(AActor* Instigator, FName ActionName);

	UFUNCTION(BlueprintCallable, Category = "Action")
	bool StartAllCancelLoopAction(AActor* Instigator);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Action")
	FORCEINLINE TArray<UCAction*> GetActions() const { return Actions; }

	UFUNCTION(BlueprintCallable, Category = "Combo")
	void ComboReset(UCAction* InAction, FName ResetName);

protected:
	UFUNCTION(Reliable, Server)
	void ServerStartAction(AActor* Instigator, FName ActionName);

	UFUNCTION(Reliable, Server)
	void ServerStopAction(AActor* Instigator, FName ActionName);

public:
	UPROPERTY(BlueprintAssignable)
	FOnActionCreateFinished OnActionCreateFinished;

	UPROPERTY(BlueprintReadWrite, Category = "Montage")
	bool bCanStopMontagePostAction;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GameplayTag")
	FGameplayTagContainer ActiveGameplayTags;

	UPROPERTY(BlueprintAssignable)
	FOnActionStateChanged OnActionStarted;

	UPROPERTY(BlueprintAssignable)
	FOnActionStateChanged OnActionStopped;

	UPROPERTY(BlueprintReadWrite)
	UCAction* ActiveMontageAction;


protected:
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	TArray<TSubclassOf<UCAction>> DefaultActions;

	UPROPERTY(BlueprintReadOnly, Replicated)
	TArray<UCAction*> Actions;

};
