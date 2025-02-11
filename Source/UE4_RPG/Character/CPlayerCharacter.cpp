#include "CPlayerCharacter.h"

#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInstanceConstant.h"

#include "Global.h"
#include "Player/CPlayerController.h"
#include "Components/CAimingComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CActionComponent.h"
#include "Components/CIKComponent.h"
#include "Components/CInteractionComponent.h"
#include "Components/CAbilitySystemComponent.h"
#include "Attributes/CPlayerCharacterAttributeSet.h"
#include "Weapon/CWeapon.h"
#include "Game/CCooldownManager.h"


ACPlayerCharacter::ACPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	SetReplicates(true);
	SetReplicateMovement(true);

	CHelpers::CreateActorComponent<UCAimingComponent>(this, &AimingComp, "AimingComp");
	CHelpers::CreateActorComponent<UCStateComponent>(this, &StateComp, "StateComp");
	CHelpers::CreateActorComponent<UCActionComponent>(this, &ActionComp, "ActionComp");
	CHelpers::CreateActorComponent<UCIKComponent>(this, &IKComp, "IKComp");
	CHelpers::CreateActorComponent<UCInteractionComponent>(this, &InteractionComp, "InteractionComp");
	CHelpers::CreateActorComponent<UCAbilitySystemComponent>(this, &AbilitySystemComp, "AbilitySystemComp");

	GetMesh()->SetCollisionProfileName("PlayerMesh");
	GetCapsuleComponent()->SetCollisionProfileName("PlayerCharacter");

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->RotationRate = FRotator(0, 1080, 0);

	Cooldown_CharacterChange = 2.f;
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

	// Create Dynamic Material
	for (int32 i = 0; i < GetMesh()->GetMaterials().Num(); i++)
	{
		UMaterialInterface* Material = GetMesh()->GetMaterial(i);
		UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(Material, this);
		if (ensure(DynamicMaterial))
		{
			GetMesh()->SetMaterial(i, DynamicMaterial);
		}
	}


	AbilitySystemComp->AttributeSet = const_cast<UCPlayerCharacterAttributeSet*>(GetAbilitySystemComponent()->GetSetChecked<UCPlayerCharacterAttributeSet>());
	if (AbilitySystemComp->AttributeSet)
	{
		if (AbilitySystemComp->OnAttributeSetFinished.IsBound())
		{
			AbilitySystemComp->OnAttributeSetFinished.Broadcast(AbilitySystemComp);
		}
	}

	CooldownManager_CharacterChange = NewObject<UCCooldownManager>(this);
	CooldownManager_CharacterChange->SetLocalMode();

	if (WeaponClass)
	{
		Weapon = GetWorld()->SpawnActorDeferred<ACWeapon>(WeaponClass, GetActorTransform(), this, this);
		Weapon->FinishSpawning(GetActorTransform());
		Weapon->ActorAttachTo(WeaponSocket);
	}

	GetActionComponent()->OnActionCreateFinished.AddDynamic(this, &ACPlayerCharacter::OnActionCreateFinished);
}

void ACPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CooldownManager_CharacterChange)
	{
		CooldownManager_CharacterChange->CooldownTick(DeltaTime);
	}

	CLog::Print(CanMoveCount, -1, DeltaTime);
}

//bool ACPlayerCharacter::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
//{
//	bool bChangedSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
//
//
//	if (CooldownManager_CharacterChange)
//	{
//		bChangedSomething |= Channel->ReplicateSubobject(CooldownManager_CharacterChange, *Bunch, *RepFlags);
//	}
//		
//
//	return bChangedSomething;
//}


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

void ACPlayerCharacter::SetCharacterChangeCooldown()
{
	
	if (FMath::IsNearlyZero(Cooldown_CharacterChange)) return;
		
	CooldownManager_CharacterChange->StartCooldown(Cooldown_CharacterChange);
	
}

void ACPlayerCharacter::ServerStopAnimMontage_Implementation(UAnimMontage* AnimMontage)
{
	StopAnimMontage(AnimMontage);
	//CLog::Print(ActionComp->bCanStopMontagePostAction ? "True" : "Fasle");
	ActionComp->bCanStopMontagePostAction = false;
	if (Weapon) Weapon->OnUnequip();
	NetMulticastStopAnimMontage(AnimMontage);
}

void ACPlayerCharacter::NetMulticastStopAnimMontage_Implementation(UAnimMontage* AnimMontage)
{
	StopAnimMontage(AnimMontage);
	if (Weapon) Weapon->OnUnequip();
	ActionComp->bCanStopMontagePostAction = false;
}

bool ACPlayerCharacter::GetCanCharacterChange() const
{
	return !CooldownManager_CharacterChange->IsCooldownActive();
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

void ACPlayerCharacter::Dash_Implementation(float MoveLength)
{
}

void ACPlayerCharacter::MoveToTarget_Implementation(float MoveLength, bool bDirect = false)
{
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

void ACPlayerCharacter::StartResonanceSkillReleased()
{
	ActionComp->StartActionByName(this, "ResonanceSkillReleased");
}

void ACPlayerCharacter::StartResonanceLiberation()
{
	ActionComp->StartActionByName(this, "ResonanceLiberation");
}


void ACPlayerCharacter::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACPlayerCharacter, bOnField);

	//DOREPLIFETIME(ACPlayerCharacter, bCanMove);
	DOREPLIFETIME(ACPlayerCharacter, bCanJump);
	DOREPLIFETIME(ACPlayerCharacter, OwnerController);
	DOREPLIFETIME(ACPlayerCharacter, CooldownManager_CharacterChange);
}
