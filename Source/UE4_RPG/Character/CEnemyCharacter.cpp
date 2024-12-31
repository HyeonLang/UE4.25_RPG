#include "CEnemyCharacter.h"
#include "Net/UnrealNetwork.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Global.h"
#include "Components/CStateComponent.h"
#include "Components/CNPCActionComponent.h"
#include "Components/CWorldWidgetComponent.h"
#include "Components/CAbilitySystemComponent.h"
#include "Attributes/CEnemyCharacterAttributeSet.h"
#include "UI/CScreenWidget.h"
#include "Weapon/CWeapon.h"

ACEnemyCharacter::ACEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	SetReplicates(true);
	SetReplicateMovement(true);

	CHelpers::CreateSceneComponent<UCWorldWidgetComponent>(this, &WidgetComp, "WidgetComp", RootComponent);

	CHelpers::CreateActorComponent<UCStateComponent>(this, &StateComp, "StateComp");
	CHelpers::CreateActorComponent<UCNPCActionComponent>(this, &NPCActionComp, "NPCActionComp");
	CHelpers::CreateActorComponent<UCAbilitySystemComponent>(this, &AbilitySystemComp, "AbilitySystemComp");

	GetMesh()->SetCollisionProfileName("CharacterMesh");
	GetCapsuleComponent()->SetCollisionProfileName("EnemyCharacter");
	WeaponSocket = TEXT("WeaponSocket");
	AlwaysWeaponEquip = false;
}

void ACEnemyCharacter::BeginPlay()
{
	// ��������� Attribute ����� ���� ������� ���� 
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
}
void ACEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UAbilitySystemComponent* ACEnemyCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComp;
}

void ACEnemyCharacter::OnHealthChanged(AActor* InstigatorActor, UCAbilitySystemComponent* OwningComp, float NewHealth, float Delta)
{
	

}


//void ACEnemyCharacter::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
//{
//	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
//}