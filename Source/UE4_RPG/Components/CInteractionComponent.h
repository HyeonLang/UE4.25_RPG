#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CInteractionComponent.generated.h"

class UCWorldWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAddFocusedActor, UCInteractionComponent*, OwningComp, AActor*, AddActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnRemoveFocusedActor, UCInteractionComponent*, OwningComp, AActor*, RemoveActor);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE4_RPG_API UCInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCInteractionComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool GetInteractionMode() const {	return bInteractionMode;	};

	UFUNCTION(BlueprintCallable)
	void SetInteractionMode(bool NewMode);

protected:
	void FindNearestInteractable();

public:
	UFUNCTION(BlueprintCallable)
	void PrimaryInteraction();

	//Server
protected:
	UFUNCTION(Reliable, Server)
	void ServerInteract(AActor* InFocused);

public:
	UPROPERTY(BlueprintAssignable)
	FOnAddFocusedActor OnAddFocusedActor;

	UPROPERTY(BlueprintAssignable)
	FOnRemoveFocusedActor OnRemoveFocusedActor;

protected:
	UPROPERTY(BlueprintReadWrite)
	TArray<AActor*> FocusedActors;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UCWorldWidget> DefaultWidgetClass;

	UPROPERTY(BlueprintReadWrite)
	UCWorldWidget* DefaultWidgetInstance;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Trace")
	float TraceRadius;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Trace")
	float TraceAngle;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Trace")
	TArray<TEnumAsByte<ECollisionChannel>> CollisionChannels;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Trace")
	bool bInteractionMode;
};
