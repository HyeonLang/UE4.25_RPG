#include "CFunctionLibrary.h"
#include "Components/CAbilitySystemComponent.h"
#include "Character/CPlayerCharacter.h"
#include "Player/CPlayerController.h"
#include "Components/CNPCActionComponent.h"
#include "Engine/EngineTypes.h"
#include "Global.h"

#define ISDEBUGLINE true
#define TRACEDEBUGTYPE EDrawDebugTrace::None

TArray<ACPlayerCharacter*> UCFunctionLibrary::GetPlayerCharactersByComponent(UActorComponent* Comp)
{

	ACPlayerCharacter* PlayerCharacter = Cast<ACPlayerCharacter>(Comp->GetOwner());
	if (PlayerCharacter)
	{
		ACPlayerController* PlayerController = Cast<ACPlayerController>(PlayerCharacter->GetController());
		if (PlayerController)
		{
			return PlayerController->GetPlayerCharacters();
		}
	}

	return TArray<ACPlayerCharacter*>();
}

bool UCFunctionLibrary::ApplyDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount, const FHitResult& HitResult)
{
	UCNPCActionComponent* NPCActionComp = Cast<UCNPCActionComponent>(TargetActor->GetComponentByClass(UCNPCActionComponent::StaticClass()));
	if (NPCActionComp)
	{
		NPCActionComp->StartNPCActionByName(TargetActor, TEXT("Hitted"));
	}

	UCAbilitySystemComponent* ASComp = UCAbilitySystemComponent::GetAbilitySystemComponent(TargetActor);
	if (ASComp)
	{
		
		return ASComp->ApplyHealthChange(DamageCauser, -DamageAmount, HitResult);
	}

	return false;
}

bool UCFunctionLibrary::ApplyDirectionDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount, const FHitResult& HitResult)
{
	if (ApplyDamage(DamageCauser, TargetActor, DamageAmount, HitResult))
	{
		UPrimitiveComponent* HitComp = HitResult.GetComponent();
		if (HitComp && HitComp->IsSimulatingPhysics(HitResult.BoneName))
		{
			HitComp->AddImpulseAtLocation(-HitResult.ImpactNormal * 300000.f, HitResult.ImpactPoint, HitResult.BoneName);
		}
		return true;
	}

	return false;
}

TArray<FHitResult> UCFunctionLibrary::SphereTraceForAttackTarget(TArray<AActor*> InIgnoreActors, FVector Location, float Radius, UObject* InWorld)
{
	TArray<FHitResult> TargetResults;

	TArray<FHitResult> HitResults;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(TEnumAsByte<EObjectTypeQuery>(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody)));
	//ObjectTypes.Add(TEnumAsByte<EObjectTypeQuery>(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic)));
	
	
	bool bHit = UKismetSystemLibrary::SphereTraceMultiForObjects(
		InWorld,
		Location,
		Location,
		Radius,
		ObjectTypes,
		false,
		InIgnoreActors,
		TRACEDEBUGTYPE,
		HitResults,
		true
		);

	if (!bHit) return TargetResults;

	TArray<AActor*> DetectedActor;

	for (auto HitResult : HitResults)
	{
		AActor* HittedActor = HitResult.GetActor();
		if (DetectedActor.Contains(HittedActor))
		{
			continue;
		}
		else
		{
			DetectedActor.Add(HittedActor);
			TargetResults.Add(HitResult);
		}
	}
	

	return TargetResults;
}

TArray<FHitResult> UCFunctionLibrary::CapsuleTraceForAttackTarget(TArray<AActor*> InIgnoreActors, FVector Location, float Radius, float HalfHeight, UObject* InWorld)
{
	TArray<FHitResult> TargetResults;

	TArray<FHitResult> HitResults;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(TEnumAsByte<EObjectTypeQuery>(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody)));
	//ObjectTypes.Add(TEnumAsByte<EObjectTypeQuery>(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic)));
	bool bHit = UKismetSystemLibrary::CapsuleTraceMultiForObjects(
		InWorld,
		Location,
		Location,
		Radius,
		HalfHeight,
		ObjectTypes,
		false,
		InIgnoreActors,
		TRACEDEBUGTYPE,
		HitResults,
		true
	);

	if (!bHit) return TargetResults;

	TArray<AActor*> DetectedActor;

	for (auto HitResult : HitResults)
	{
		AActor* HittedActor = HitResult.GetActor();
		if (DetectedActor.Contains(HittedActor))
		{
			continue;
		}
		else
		{
			DetectedActor.Add(HittedActor);
			TargetResults.Add(HitResult);
		}
	}

	return TargetResults;
}

TArray<FHitResult> UCFunctionLibrary::BoxTraceForAttackTarget(TArray<AActor*> InIgnoreActors, FVector Location, FVector HalfSize, FRotator Orientation, UObject* InWorld)
{

	TArray<FHitResult> TargetResults;

	TArray<FHitResult> HitResults;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(TEnumAsByte<EObjectTypeQuery>(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody)));
	//ObjectTypes.Add(TEnumAsByte<EObjectTypeQuery>(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic)));

	bool bHit = UKismetSystemLibrary::BoxTraceMultiForObjects(
		InWorld,
		Location,
		Location,
		HalfSize,
		Orientation,
		ObjectTypes,
		false,
		InIgnoreActors,
		TRACEDEBUGTYPE,
		HitResults,
		true
	);

	if (!bHit) return TargetResults;

	TArray<AActor*> DetectedActor;

	for (auto HitResult : HitResults)
	{
		AActor* HittedActor = HitResult.GetActor();
		if (DetectedActor.Contains(HittedActor))
		{
			continue;
		}
		else
		{
			DetectedActor.Add(HittedActor);
			TargetResults.Add(HitResult);
		}
	}

	return TargetResults;
}

