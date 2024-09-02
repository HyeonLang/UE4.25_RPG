#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CPlayerAttributeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE4_RPG_API UCPlayerAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCPlayerAttributeComponent();

protected:
	virtual void BeginPlay() override;

public:	
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	FORCEINLINE float GetMaxStamina() const { return MaxStamina; }
	FORCEINLINE float GetCurrentStamina() const { return CurrentStamina; }

	void SetMaxStamina(float value);
	void SetCurrentStamina(float value);

private:
	UPROPERTY(EditDefaultsOnly, Category = "Attribute")
	float MaxStamina;

	float CurrentStamina;



		
};
