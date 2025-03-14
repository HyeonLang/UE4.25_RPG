// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CAnimNotify_ActionPlayParticleEffect.generated.h"

class UAnimSequenceBase;
class UParticleSystem;
class USkeletalMeshComponent;
class UParticleSystemComponent;

UCLASS()
class UE4_RPG_API UCAnimNotify_ActionPlayParticleEffect : public UAnimNotify
{
	GENERATED_BODY()

public:
	UCAnimNotify_ActionPlayParticleEffect();

	virtual void PostLoad() override;


public:
	FString GetNotifyName_Implementation() const override;
	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;


protected:
	UParticleSystem* GetActionParticleSystem(FName InActionName, USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation);
	virtual UParticleSystemComponent* SpawnParticleSystem(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimNotify")
	int32 EffectIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimNotify")
	FName ActionName;

	// Location offset from the socket
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimNotify")
	FVector LocationOffset;

	// Rotation offset from socket
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimNotify")
	FRotator RotationOffset;

	// Scale to spawn the particle system at
	UPROPERTY(EditAnywhere, Category = "AnimNotify")
	FVector Scale;

private:
	// Cached version of the Rotation Offset already in Quat form
	FQuat RotationOffsetQuat;

public:

	// Should attach to the bone/socket
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimNotify")
	uint32 Attached : 1; 	//~ Does not follow coding standard due to redirection from BP

	// SocketName to attach to
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimNotify")
	FName SocketName;


};
