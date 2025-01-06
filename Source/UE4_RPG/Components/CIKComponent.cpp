#include "CIKComponent.h"
#include "Global.h"
#include "Character/CPlayerCharacter.h"
#include "Player/CPlayerController.h"
#include "Player/CPlayerCameraActor.h"
#include "Components/CapsuleComponent.h"

//static TAutoConsoleVariable<bool> CVarIkMode(TEXT("RPG.IkMode"), true, TEXT("Enable IK Mode"), ECVF_Cheat);

UCIKComponent::UCIKComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	LeftFootSocketName = "Bip001LFoot";
	RightFootSocketName = "Bip001RFoot";

	DrawDebugType = EDrawDebugTrace::ForOneFrame;

	Additional = 40.f;
	FootHeight = 0.f;
	InterpSpeed = 16.f;

}


void UCIKComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
	OwnerPlayerCharacter = Cast<ACPlayerCharacter>(OwnerCharacter);
	
	CapsuleHalfHeight = OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
}


void UCIKComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!IsIKMode()) return;

	float LeftDistance;
	FRotator LeftRotation;
	IKTrace(LeftFootSocketName, LeftDistance, LeftRotation);

	float RightDistance;
	FRotator RightRotation;
	IKTrace(RightFootSocketName, RightDistance, RightRotation);

	float Floating = FMath::Min(LeftDistance, RightDistance);

	IKFeetData.PelvisDistance.Z = UKismetMathLibrary::FInterpTo(IKFeetData.PelvisDistance.Z, Floating, DeltaTime, InterpSpeed);

	IKFeetData.LeftDistance.X = UKismetMathLibrary::FInterpTo(IKFeetData.LeftDistance.X, -(LeftDistance - Floating), DeltaTime, InterpSpeed);
	IKFeetData.RightDistance.X = UKismetMathLibrary::FInterpTo(IKFeetData.RightDistance.X, -(RightDistance - Floating), DeltaTime, InterpSpeed);

	IKFeetData.LeftRotation = UKismetMathLibrary::RInterpTo(IKFeetData.LeftRotation, LeftRotation, DeltaTime, InterpSpeed);
	IKFeetData.RightRotation = UKismetMathLibrary::RInterpTo(IKFeetData.RightRotation, RightRotation, DeltaTime, InterpSpeed);
}

void UCIKComponent::SetIKMode(bool NewIkMode)
{
	bIKMode = NewIkMode;
}

void UCIKComponent::IKTrace_Implementation(FName InSocketName, float& OutDistance, FRotator& OutRotation)
{
	OutDistance = 0.f;

	FVector SocketLocation = OwnerCharacter->GetMesh()->GetSocketLocation(InSocketName);
	FVector Start = FVector(SocketLocation.X, SocketLocation.Y, OwnerCharacter->GetActorLocation().Z);

	float TraceZ = Start.Z - CapsuleHalfHeight - Additional;
	FVector End = FVector(SocketLocation.X, SocketLocation.Y, TraceZ);

	TArray<AActor*> Ignores;
	Ignores.Add(OwnerCharacter);

	if (OwnerPlayerCharacter && OwnerPlayerCharacter->GetOwnerController())
	{
		for (ACPlayerCharacter* PC : OwnerPlayerCharacter->GetOwnerController()->GetPlayerCharacters())
		{
			if (!PC) continue;
			if (OwnerCharacter == PC) continue;

			Ignores.Add(PC);
		}

		if (OwnerPlayerCharacter->GetOwnerController()->GetPlayerCameraActor())
		{
			Ignores.Add(OwnerPlayerCharacter->GetOwnerController()->GetPlayerCameraActor());
		}
	}

	FHitResult Hit;
	UKismetSystemLibrary::LineTraceSingle
	(
		GetWorld(),
		Start,
		End,
		UEngineTypes::ConvertToTraceType(ECC_Visibility),
		true,
		Ignores,
		DrawDebugType,
		Hit,
		true
	);

	CheckFalse(Hit.bBlockingHit);

	float Length = (Hit.ImpactPoint - Hit.TraceEnd).Size();
	OutDistance = FootHeight + Length - Additional;

	FVector ImpactNormal = Hit.ImpactNormal;

	float Pitch = -UKismetMathLibrary::DegAtan2(ImpactNormal.X, ImpactNormal.Z);
	float Roll = UKismetMathLibrary::DegAtan2(ImpactNormal.Y, ImpactNormal.Z);

	Pitch = FMath::Clamp(Pitch, -30.f, 30.f);
	Roll = FMath::Clamp(Roll, -15.f, 15.f);

	OutRotation = FRotator(Pitch, 0.f, Roll);
}

