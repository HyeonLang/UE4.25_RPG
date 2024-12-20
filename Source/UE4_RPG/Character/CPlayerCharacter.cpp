#include "CPlayerCharacter.h"

#include "Net/UnrealNetwork.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Global.h"
#include "Player/CPlayerController.h"
#include "Components/CAimingComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CActionComponent.h"
#include "Components/CAbilitySystemComponent.h"
#include "Attributes/CPlayerCharacterAttributeSet.h"
#include "Weapon/CWeapon.h"


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
	bCanJump = true;
	//bCanMove = true;
	CanMoveCount = 0;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	
	GetMesh()->MeshComponentUpdateFlag = (uint8)EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
	WeaponSocket = TEXT("WeaponProp02_Socket");
}

void ACPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	AbilitySystemComp->AttributeSet = const_cast<UCPlayerCharacterAttributeSet*>(GetAbilitySystemComponent()->GetSetChecked<UCPlayerCharacterAttributeSet>());
	if (AbilitySystemComp->AttributeSet)
	{
		if (AbilitySystemComp->OnAttributeSetFinished.IsBound())
		{
			AbilitySystemComp->OnAttributeSetFinished.Broadcast(AbilitySystemComp);
		}
	}

	if (WeaponClass)
	{
		Weapon = GetWorld()->SpawnActor<ACWeapon>(WeaponClass, GetActorTransform());
		Weapon->SetOwner(this);
		Weapon->ActorAttachTo(WeaponSocket);
	}

	GetActionComponent()->OnActionCreateFinished.AddDynamic(this, &ACPlayerCharacter::OnActionCreateFinished);
}

void ACPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


UAbilitySystemComponent* ACPlayerCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComp;
}

TSoftObjectPtr<UTexture2D> ACPlayerCharacter::GetIcon_Implementation() const
{
	return Icon;
}

void ACPlayerCharacter::SetOwnerController(ACPlayerController* InController)
{
	OwnerController = InController;
}

void ACPlayerCharacter::OnMoveForward(float Axis)
{
	if (CanMoveCount) return;

	if (IsActiveMontage && !FMath::IsNearlyZero(Axis) && ActionComp->bCanStopMontagePostAction)
	{
		ServerStopAnimMontage();
		Weapon->OnUnequip();
	}

	FRotator ControlRotation = FRotator(0, GetControlRotation().Yaw, 0);
	FVector direction = FQuat(ControlRotation).GetForwardVector();

	AddMovementInput(direction, Axis);
}

void ACPlayerCharacter::OnMoveRight(float Axis)
{
	if (CanMoveCount) return;
	
	if (IsActiveMontage && !FMath::IsNearlyZero(Axis) && ActionComp->bCanStopMontagePostAction)
	{
		Weapon->OnUnequip();
		ServerStopAnimMontage();
	}

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
	if (CanMoveCount) return;
	ActionComp->StartActionByName(this, "Jump");
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

void ACPlayerCharacter::ServerStopAnimMontage_Implementation(UAnimMontage* AnimMontage)
{
	StopAnimMontage(AnimMontage);
	//CLog::Print(ActionComp->bCanStopMontagePostAction ? "True" : "Fasle");
	ActionComp->bCanStopMontagePostAction = false;
	NetMulticastStopAnimMontage(AnimMontage);
}

void ACPlayerCharacter::NetMulticastStopAnimMontage_Implementation(UAnimMontage* AnimMontage)
{
	StopAnimMontage(AnimMontage);
	ActionComp->bCanStopMontagePostAction = false;
}

void ACPlayerCharacter::SetAllVisibility(bool bNewVisibility)
{
	GetMesh()->SetVisibility(bNewVisibility);
	if (Weapon)
	{
		if (bNewVisibility)
			Weapon->OnEquip_Implementation();
		else
			Weapon->OnUnequip_Implementation();
	}
}

void ACPlayerCharacter::SetOnField(bool InNew)
{
	bOnField = InNew;
}

void ACPlayerCharacter::SetCanJump(bool InNew)
{
	bCanJump = InNew;
}

void ACPlayerCharacter::SetCanMove(bool InNew)
{
	CanMoveCount = InNew;
}

void ACPlayerCharacter::OnActionCreateFinished_Implementation(UCActionComponent* OwningComp)
{
	if (OwnerController)
	{
		for (int i = 0; i < OwnerController->GetPlayerCharacters().Num(); i++)
		{
			if (OwnerController->GetPlayerCharacters()[i]->GetActionComponent() == OwningComp)
			{

			}
		}
	}
}

void ACPlayerCharacter::OnRep_OnField()
{
	
	if (bOnField)
	{
		//GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		GetCapsuleComponent()->SetCollisionProfileName("PlayerCharacter");
		GetMesh()->SetVisibility(true);
	}
	else
	{
		//GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetCapsuleComponent()->SetCollisionProfileName("BehindPlayerCharacter");
		SetAllVisibility(false);
	}
}

void ACPlayerCharacter::StartSprint()
{
	ActionComp->StartActionByName(this, "Sprint");
}

void ACPlayerCharacter::StopSprint()
{
	ActionComp->StopActionByName(this, "Sprint");
}

void ACPlayerCharacter::StartNormalAttack()
{
	ActionComp->StartActionByName(this, "NormalAttack");
}

void ACPlayerCharacter::StartResonanceSkill()
{
	ActionComp->StartActionByName(this, "ResonanceSkill");
}

void ACPlayerCharacter::StartResonanceLiberation()
{
	ActionComp->StartActionByName(this, "ResonanceLiberation");
}


void ACPlayerCharacter::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACPlayerCharacter, bOnField);

	DOREPLIFETIME(ACPlayerCharacter, Cooldown_CharacterChange);
	DOREPLIFETIME(ACPlayerCharacter, bCanCharacterChange);
	//DOREPLIFETIME(ACPlayerCharacter, bCanMove);
	DOREPLIFETIME(ACPlayerCharacter, bCanJump);
	DOREPLIFETIME(ACPlayerCharacter, OwnerController);
}
