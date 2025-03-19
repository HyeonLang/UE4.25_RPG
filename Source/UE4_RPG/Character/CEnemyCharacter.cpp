#include "CEnemyCharacter.h"
#include "Net/UnrealNetwork.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Global.h"
#include "Components/CStateComponent.h"
#include "Components/CNPCActionComponent.h"
#include "Components/CWorldWidgetComponent.h"
#include "Components/CAbilitySystemComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "Attributes/CEnemyCharacterAttributeSet.h"
#include "UI/CScreenWidget.h"
#include "Weapon/CWeapon.h"

ACEnemyCharacter::ACEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	SetReplicates(true);
	SetReplicateMovement(true);

	CHelpers::CreateSceneComponent<UCWorldWidgetComponent>(this, &WidgetComp, "WidgetComp", RootComponent);
	
	CHelpers::CreateActorComponent<UPawnSensingComponent>(this, &PawnSensingComp, "PawnSensingComp");
	CHelpers::CreateActorComponent<UCStateComponent>(this, &StateComp, "StateComp");
	CHelpers::CreateActorComponent<UCNPCActionComponent>(this, &NPCActionComp, "NPCActionComp");
	CHelpers::CreateActorComponent<UCAbilitySystemComponent>(this, &AbilitySystemComp, "AbilitySystemComp");

	GetMesh()->SetCollisionProfileName("CharacterMesh");
	GetCapsuleComponent()->SetCollisionProfileName("EnemyCharacter");
	WeaponSocket = TEXT("WeaponSocket");
	AlwaysWeaponEquip = false;
	SetCanBeDamaged(true);

	TargetActorKeyName = "TargetActor";
}

void ACEnemyCharacter::BeginPlay()
{
	for (int32 i = 0; i < GetMesh()->GetMaterials().Num(); i++)
	{
		UMaterialInterface* Material = GetMesh()->GetMaterial(i);
		UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(Material, this);
		if (ensure(DynamicMaterial))
		{
			DynamicMaterial->SetScalarParameterValue("Amount", 0.f);
			GetMesh()->SetMaterial(i, DynamicMaterial);
		}
	}

	// 위젯등에서의 Attribute 사용을 위해 가장먼저 생성 
	AbilitySystemComp->AttributeSet = const_cast<UCEnemyCharacterAttributeSet*>(GetAbilitySystemComponent()->GetSetChecked<UCEnemyCharacterAttributeSet>());

	Super::BeginPlay();

	WidgetComp->SetRelativeLocation(FVector(0, 0, GetCapsuleComponent()->GetScaledCapsuleHalfHeight()));

	if (WeaponClass)
	{
		Weapon = GetWorld()->SpawnActorDeferred<ACWeapon>(WeaponClass, GetActorTransform(), this,  this);
		Weapon->bAlwaysEquip = AlwaysWeaponEquip;
		Weapon->FinishSpawning(GetActorTransform());
		Weapon->ActorAttachTo(WeaponSocket);
	}

	AbilitySystemComp->OnHealthChanged.AddDynamic(this, &ACEnemyCharacter::OnHealthChanged);
	//PawnSensingComp->OnSeePawn.AddDynamic(this, &ACEnemyCharacter::OnSeePawn);
}
void ACEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

UAbilitySystemComponent* ACEnemyCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComp;
}

void ACEnemyCharacter::SetTargetActor(AActor* NewTarget)
{
	AAIController* AIC = GetController<AAIController>();
	if (AIC)
	{
		AIC->GetBlackboardComponent()->SetValueAsObject(TargetActorKeyName, NewTarget);
	}
}

AActor* ACEnemyCharacter::GetTargetActor() const
{
	AAIController* AIC = GetController<AAIController>();
	if (AIC)
	{
		return Cast<AActor>(AIC->GetBlackboardComponent()->GetValueAsObject(TargetActorKeyName));
	}

	return nullptr;
}

void ACEnemyCharacter::OnHealthChanged(AActor* InstigatorActor, UCAbilitySystemComponent* OwningComp, float NewHealth, float Delta)
{
	if (NewHealth <= 0.f)
	{
		SetCanBeDamaged(false);
		NPCActionComp->StartNPCActionByName(this, TEXT("Dead"));
	}

}

void ACEnemyCharacter::OnSeePawn(APawn* Pawn)
{
	AAIController* AIC = GetController<AAIController>();
	if (ensure(AIC))
	{
		if (GetTargetActor() != Pawn)
		{
			SetTargetActor(Pawn);

			NetMulticastPawnSeen();
		}
	}
}

void ACEnemyCharacter::NetMulticastPawnSeen_Implementation()
{
}

void ACEnemyCharacter::Dead_Implementation()
{
	if (Weapon && !Weapon->IsPendingKill())
	{
		Weapon->Destroy();
	}
}


//void ACEnemyCharacter::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
//{
//	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
//}