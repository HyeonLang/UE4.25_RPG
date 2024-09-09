#include "CPlayerCharacter.h"

#include "Net/UnrealNetwork.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "../Global.h"
#include "../Player/CPlayerController.h"
#include "../Components/CAimingComponent.h"
#include "../Components/CStateComponent.h"
#include "../Components/CActionComponent.h"
#include "Components/CAbilitySystemComponent.h"
#include "Attributes/CPlayerCharacterAttributeSet.h"


ACPlayerCharacter::ACPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateActorComponent<UCAimingComponent>(this, &AimingComp, "AimingComp");
	CHelpers::CreateActorComponent<UCStateComponent>(this, &StateComp, "StateComp");
	CHelpers::CreateActorComponent<UCActionComponent>(this, &ActionComp, "ActionComp");
	CHelpers::CreateActorComponent<UCAbilitySystemComponent>(this, &AbilitySystemComp, "AbilitySystemComp");

	GetMesh()->SetCollisionProfileName("PlayerMesh");
	GetCapsuleComponent()->SetCollisionProfileName("PlayerCharacter");

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->RotationRate = FRotator(0, 1080, 0);

	Cooldown_CharacterChange = 2.f;
	bCanCharacterChange = true;
	bOnField = true;
}

void ACPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (GetController() && GetController()->IsLocalController())
		CLog::Print(GetCapsuleComponent()->GetCollisionEnabled(),-1,0.f,FColor::Blue);

}


UAbilitySystemComponent* ACPlayerCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComp;
}

void ACPlayerCharacter::OnMoveForward(float Axis)
{
	FRotator ControlRotation = FRotator(0, GetControlRotation().Yaw, 0);
	FVector direction = FQuat(ControlRotation).GetForwardVector();

	AddMovementInput(direction, Axis);
}

void ACPlayerCharacter::OnMoveRight(float Axis)
{
	FRotator ControlRotation = FRotator(0, GetControlRotation().Yaw, 0);
	FVector direction = FQuat(ControlRotation).GetRightVector();

	AddMovementInput(direction, Axis);
}

void ACPlayerCharacter::OnTurn(float Axis)
{
	float Rate = 45.f * Axis * GetWorld()->GetDeltaSeconds();

	AddControllerYawInput(Rate);
}

void ACPlayerCharacter::OnLookUp(float Axis)
{
	float Rate = 45.f * Axis * GetWorld()->GetDeltaSeconds();

	AddControllerPitchInput(Rate);
}

void ACPlayerCharacter::OnJump()
{
	if (!bCanJump) return;
	Jump();
}

void ACPlayerCharacter::OnDash()
{
	//ActionComp->SetDashMode();
}

void ACPlayerCharacter::SetCharacterChangeCooldown_Implementation()
{
	FTimerDelegate Delegate;
	Delegate.BindUFunction(this, "SetCanCharacterChange", true);
	GetWorldTimerManager().SetTimer(TimerHandle_Cooldown_CharacterChange, Delegate, Cooldown_CharacterChange, false);
}

void ACPlayerCharacter::SetCanCharacterChange_Implementation(bool InNew)
{
	bCanCharacterChange = InNew;
}

void ACPlayerCharacter::SetOnField(bool InNew)
{
	bOnField = InNew;
}

void ACPlayerCharacter::OnRep_OnField()
{
	
	if (bOnField)
	{
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		GetMesh()->SetVisibility(true);
	}
	else
	{
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetMesh()->SetVisibility(false);
	}
}

void ACPlayerCharacter::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACPlayerCharacter, bOnField);

	DOREPLIFETIME(ACPlayerCharacter, Cooldown_CharacterChange);
	DOREPLIFETIME(ACPlayerCharacter, bCanCharacterChange);
}
