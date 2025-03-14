#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CFunctionLibrary.generated.h"

class ACPlayerCharacter;

UENUM(BlueprintType)
enum class EGameAttributeType : uint8
{
	Normal, Glacio, Fusion, Conducto, Aero, Spectro, Havoc, Heal, Max
};


UCLASS(Blueprintable)
class UE4_RPG_API UCFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "FunctionLibrary")
	static TArray<ACPlayerCharacter*> GetPlayerCharactersByComponent(UActorComponent* Comp);

	UFUNCTION(BlueprintCallable, Category = "FunctionLibrary")
	static bool ApplyDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount, const FHitResult& HitResult);

	UFUNCTION(BlueprintCallable, Category = "FunctionLibrary")
	static bool ApplyDirectionDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount, const FHitResult& HitResult);

	UFUNCTION(BlueprintCallable, Category = "FunctionLibrary")
	static TArray<FHitResult> SphereTraceForAttackTarget(TArray<AActor*> InIgnoreActors, FVector Location, float Radius, UObject* InWorld);

	UFUNCTION(BlueprintCallable, Category = "FunctionLibrary")
	static TArray<FHitResult> SphereTracesForAttackTarget(TArray<AActor*> InIgnoreActors, FVector StartLocation, FVector EndLocation, float Radius, UObject* InWorld);

	UFUNCTION(BlueprintCallable, Category = "FunctionLibrary")
	static TArray<FHitResult> CapsuleTraceForAttackTarget(TArray<AActor*> InIgnoreActors, FVector Location, float Radius, float HalfHeight, UObject* InWorld);

	UFUNCTION(BlueprintCallable, Category = "FunctionLibrary")
	static TArray<FHitResult> BoxTraceForAttackTarget(TArray<AActor*> InIgnoreActors, FVector Location, FVector HalfSize, FRotator Orientation, UObject* InWorld);


	UFUNCTION(BlueprintCallable, Category = "FunctionLibrary")
	static FColor GetAttributeColor(EGameAttributeType InGameAttributeType);

	UFUNCTION(BlueprintCallable, Category = "FunctionLibrary")
	static FLinearColor GetAttributeLinearColor(EGameAttributeType InGameAttributeType);
	
};
