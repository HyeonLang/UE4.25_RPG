#include "CFunctionLibrary.h"
#include "Components/CAbilitySystemComponent.h"
#include "Global.h"

#define ISDEBUGLINE true

bool UCFunctionLibrary::ApplyDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount)
{
	UCAbilitySystemComponent* ASComp = UCAbilitySystemComponent::GetAbilitySystemComponent(TargetActor);
	if (ASComp)
	{
		return ASComp->ApplyHealthChange(DamageCauser, -DamageAmount);
	}

	return false;
}

bool UCFunctionLibrary::ApplyDirectionDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount, const FHitResult& HitResult)
{
	if (ApplyDamage(DamageCauser, TargetActor, DamageAmount))
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

TArray<FHitResult> UCFunctionLibrary::SphereTraceForAttackTarget(TArray<AActor*>& InIgnoreActors, FVector Location, float Radius, UObject* InWorld)
{
	TArray<FHitResult> HitResults;

	bool bHit = UKismetSystemLibrary::SphereTraceMulti(
		InWorld,
		Location,
		Location,
		Radius,
		UEngineTypes::ConvertToTraceType(ECC_Visibility),
		false,
		InIgnoreActors,
		EDrawDebugTrace::ForDuration,
		HitResults,
		ISDEBUGLINE
		);
	
	return HitResults;
}

