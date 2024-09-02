#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "CActionComponent.generated.h"

class UCAction;

//UENUM(BlueprintType)
//enum class EActionType : uint8
//{
//	Idle, Dash, Action, Ultimate, Hitted, Stun, Dying, Dead, Max
//};

//DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FActionTypeChanged, EActionType, InPrevType, EActionType, InNewType);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE4_RPG_API UCActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCActionComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UFUNCTION(BlueprintCallable, Category = "Action")
		void AddAction(AActor* Instigator, TSubclassOf<UCAction> ActionClass);

	UFUNCTION(BlueprintCallable, Category = "Action")
		void RemoveAction(UCAction* ActionToRemove);

	UFUNCTION(BlueprintCallable, Category = "Action")
		bool StartActionByName(AActor* Instigator, FName ActionName);

	UFUNCTION(BlueprintCallable, Category = "Action")
		bool StopActionByName(AActor* Instigator, FName ActionName);

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GameplayTag")
		FGameplayTagContainer ActiveGamePlayTags;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Action")
		TArray<TSubclassOf<UCAction>> DefaultActions;

	UPROPERTY()
		TArray<UCAction*> Actions;

};
