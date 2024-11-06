#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CPlayerController.generated.h"

class ACharacter;
class ACPlayerCharacter;
class ACPlayerCameraActor;
class UCPlayerAttributeComponent;
class USceneComponent;
class UCameraComponent;
class USpringArmComponent;


UENUM(BlueprintType)
enum class EChangeMode : uint8
{
	None, Action, Concerto, Max
};

UCLASS()
class UE4_RPG_API ACPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ACPlayerController();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

public:
	virtual void Tick(float DeltaSeconds) override;

public:
	FORCEINLINE int32 GetMaxPlayerCharacterCount() { return MaxPlayerCharacterCount; }
	FORCEINLINE const TArray<TSubclassOf<ACPlayerCharacter>> GetCharacterClasses() { return CharacterClasses; }
	FORCEINLINE TArray<ACPlayerCharacter*>& GetPlayerCharacters() { return PlayerCharacters; }
	FORCEINLINE const int32 GetPlayerCharacterCurrentIndex() { return PlayerCharacterCurrentIndex; }
	FORCEINLINE ACPlayerCameraActor* GetPlayerCameraActor() { return PlayerCameraActor; }
	

	void SetPlayerCharacterCurrentIndex(int32 InIndex);

	void AddControlledPlayerCharacter(ACPlayerCharacter* InNewCharacter);

	void SpawnPlayerCharacter(FTransform StartTransform);

	void SpawnCameraActor(FTransform StartTransform);

	void PossessCharacter(ACPlayerCharacter* InNewCharacter, EChangeMode InMode);

	UFUNCTION(Reliable, Server)
	void ServerPossessCharacter(ACPlayerCharacter* InNewCharacter, EChangeMode InMode);

	void UnPossessCharacter(EChangeMode InMode);

	UFUNCTION(Reliable, Server)
	void ServerUnPossessCharacter(EChangeMode InMode);

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastUnPossessCharacter(EChangeMode InMode);

	bool GetChangeCharacterLocation(FVector& OutLocation, AActor* PlayerActor, AActor* TargetActor = nullptr, EChangeMode InMode = EChangeMode::None) const;

private:
	void OnInputKey_R();
	void OnInputKey_F();
	void OnInputKey_Q();
	void OnInputKey_E();

	void OnInputForward(float Axis);
	void OnInputRight(float Axis);

	void OnSpaceBar();

	void OnInputKey_1();
	void OnInputKey_2();
	void OnInputKey_3();
	void OnInputKey_4();

	void OnInputKey_C();
	void OnInputKey_Z();

	void OnPrimaryAction();
	void OnSecondaryAction();

	void OnInputKey_Shift();

	void OnInputKey_ESC();

	void OnInputMouse_Left(float Axis);
	void OnInputMouse_Right(float Axis);

	void OnInputMouse_Wheel(float Axis);

	void OnMouseX(float Axis);
	void OnMouseY(float Axis);

	void OnStartSprint();
	void OnStopSprint();

private:
	void ChangePlayerCharacter(uint32 InIndex);

	void HideCharacter(ACPlayerCharacter* Character);
	UFUNCTION()
	void OnActionStopped_HideCharacter(UCActionComponent* OwningComp, UCAction* Action);
	void ShowCharacter(ACPlayerCharacter* Character);

protected:
	UFUNCTION()
	void OnSetViewTarget();

public:
	FORCEINLINE ACPlayerCameraActor* GetPlayerCameraActor() const { return PlayerCameraActor; }

private:
	UPROPERTY(EditDefaultsOnly, Category = "Character")
	int32 MaxPlayerCharacterCount;

	UPROPERTY(EditDefaultsOnly, Category = "Character")
	TArray<TSubclassOf<ACPlayerCharacter>> CharacterClasses;

	UPROPERTY(EditDefaultsOnly, Category = "PlayerCameraActor")
	TSubclassOf<ACPlayerCameraActor> PlayerCameraActorClass;

	UPROPERTY(VisibleDefaultsOnly, Category = "Component")
	UCPlayerAttributeComponent* PlayerAttributeComp;

public:
	UPROPERTY(Replicated, VisibleAnywhere)
	ACPlayerCameraActor* PlayerCameraActor;

private:
	UPROPERTY(Replicated, VisibleAnywhere)
	ACPlayerCharacter* PlayerCharacter;

	UPROPERTY(Replicated, VisibleAnywhere)
	TArray<ACPlayerCharacter*> PlayerCharacters;

	UPROPERTY(Replicated)
	int32 PlayerCharacterCurrentIndex;

	FVector CurrentLocation;
	FRotator CurrentRotation;
	FRotator CameraRotation;


};
