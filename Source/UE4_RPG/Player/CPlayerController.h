#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CPlayerController.generated.h"

class ACharacter;
class ACPlayerCharacter;
class ACPlayerCameraActor;
class UCPlayerAttributeComponent;

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

	void SetPlayerCharacterCurrentIndex(int32 InIndex);

	void AddControlledPlayerCharacter(ACPlayerCharacter* InNewCharacter);

	void SpawnPlayerCharacter(FTransform StartTransform);
	void PossessCharacter(ACPlayerCharacter* InNewCharacter, EChangeMode InMode);
	void UnPossessCharacter(EChangeMode InMode);

private:
	void OnInputKey_R();
	void OnInputKey_F();
	void OnInputKey_Q();
	void OnInputKey_E();

	void OnInputForward(float Axis);
	void OnInputRight(float Axis);

	void OnJump();

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

private:
	void ChangePlayerCharacter(uint32 InIndex);

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

private:
	UPROPERTY(VisibleAnywhere)
	ACPlayerCharacter* PlayerCharacter;
	UPROPERTY(VisibleAnywhere)
	ACPlayerCameraActor* PlayerCameraActor;

	UPROPERTY(VisibleAnywhere)
	TArray<ACPlayerCharacter*> PlayerCharacters;

	int32 PlayerCharacterCurrentIndex;

	FVector CurrentLocation;
	FRotator CurrentRotation;
	FRotator CameraRotation;
};
