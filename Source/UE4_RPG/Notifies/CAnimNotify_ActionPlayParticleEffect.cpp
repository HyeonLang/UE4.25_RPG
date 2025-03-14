// Fill out your copyright notice in the Description page of Project Settings.


#include "CAnimNotify_ActionPlayParticleEffect.h"

#include "Particles/ParticleSystem.h"
#include "Components/SkeletalMeshComponent.h"

#include "Animation/AnimSequenceBase.h"

#include "Global.h"
#include "Character/CPlayerCharacter.h"
#include "Character/CEnemyCharacter.h"
#include "Components/CActionComponent.h"
#include "Components/CNPCActionComponent.h"
#include "Actions/CNPCAction.h"
#include "Actions/CAction.h"

UCAnimNotify_ActionPlayParticleEffect::UCAnimNotify_ActionPlayParticleEffect()
	: Super()
{
	Attached = true;
	Scale = FVector(1.f);
	
	EffectIndex = 0;


#if WITH_EDITORONLY_DATA
	NotifyColor = FColor(192, 255, 99, 255);
#endif // WITH_EDITORONLY_DATA
}

void UCAnimNotify_ActionPlayParticleEffect::PostLoad()
{
	Super::PostLoad();

	RotationOffsetQuat = FQuat(RotationOffset);
}


FString UCAnimNotify_ActionPlayParticleEffect::GetNotifyName_Implementation() const
{
	return "CActionPlayParticleEffect";
}

void UCAnimNotify_ActionPlayParticleEffect::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	SpawnParticleSystem(MeshComp, Animation);
}

UParticleSystem* UCAnimNotify_ActionPlayParticleEffect::GetActionParticleSystem(FName InActionName, USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	TArray<FActionData> ActionDatas;
	UCActionBase* Action = nullptr;
	int32 ComboIndex = 0;

	ACPlayerCharacter* PlayerCharacter = Cast<ACPlayerCharacter>(MeshComp->GetOwner());
	if (PlayerCharacter)
	{
		Action = PlayerCharacter->GetActionComponent()->GetActionByName(InActionName);
	}
	else
	{
		ACEnemyCharacter* EnemyCharacter = Cast<ACEnemyCharacter>(MeshComp->GetOwner());
		if (EnemyCharacter)
		{
			Action = PlayerCharacter->GetActionComponent()->GetActionByName(InActionName);
		}
	}

	if (Action)
	{
		ActionDatas = Action->ActionDatas;
		if (ActionDatas.IsValidIndex(Action->ComboIndex) && ActionDatas[Action->ComboIndex].EffectDatas.IsValidIndex(EffectIndex))
		{
			return ActionDatas[Action->ComboIndex].EffectDatas[EffectIndex].Effects;
		}
	}

	return nullptr;
}

UParticleSystemComponent* UCAnimNotify_ActionPlayParticleEffect::SpawnParticleSystem(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	UParticleSystemComponent* ReturnComp = nullptr;
	UParticleSystem* PSTemplate = GetActionParticleSystem(ActionName, MeshComp, Animation);

	if (PSTemplate)
	{
		if (PSTemplate->IsLooping())
		{
			//UE_LOG(LogParticles, Warning, TEXT("Particle Notify: Anim '%s' tried to spawn infinitely looping particle system '%s'. Spawning suppressed."), *GetNameSafe(Animation), *GetNameSafe(PSTemplate));
			return ReturnComp;
		}

		if (Attached)
		{
			ReturnComp = UGameplayStatics::SpawnEmitterAttached(PSTemplate, MeshComp, SocketName, LocationOffset, RotationOffset, Scale);
		}
		else
		{
			const FTransform MeshTransform = MeshComp->GetSocketTransform(SocketName);
			FTransform SpawnTransform;
			SpawnTransform.SetLocation(MeshTransform.TransformPosition(LocationOffset));
			SpawnTransform.SetRotation(MeshTransform.GetRotation() * RotationOffsetQuat);
			SpawnTransform.SetScale3D(Scale);
			ReturnComp = UGameplayStatics::SpawnEmitterAtLocation(MeshComp->GetWorld(), PSTemplate, SpawnTransform);
		}
	}
	else
	{
		//UE_LOG(LogParticles, Warning, TEXT("Particle Notify: Particle system is null for particle notify '%s' in anim: '%s'"), *GetNotifyName(), *GetPathNameSafe(Animation));
	}

	return ReturnComp;
}
