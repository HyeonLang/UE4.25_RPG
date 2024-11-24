#include "CEnemyCharacter.h"
#include "Net/UnrealNetwork.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "../Global.h"
#include "../Components/CStateComponent.h"
#include "../Components/CActionComponent.h"
#include "Components/CAbilitySystemComponent.h"
#include "Attributes/CEnemyCharacterAttributeSet.h"
#include "UI/CWorldWidget.h"

ACEnemyCharacter::ACEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateActorComponent<UCStateComponent>(this, &StateComp, "StateComp");
	CHelpers::CreateActorComponent<UCActionComponent>(this, &ActionComp, "ActionComp");
	CHelpers::CreateActorComponent<UCAbilitySystemComponent>(this, &AbilitySystemComp, "AbilitySystemComp");
}

void ACEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	AbilitySystemComp->AttributeSet = const_cast<UCEnemyCharacterAttributeSet*>(GetAbilitySystemComponent()->GetSetChecked<UCEnemyCharacterAttributeSet>());


	OnAttackBegin();
}
void ACEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

UAbilitySystemComponent* ACEnemyCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComp;
}

void ACEnemyCharacter::OnAttackBegin_Implementation()
{
	// HP bar Create
	if (HealthBarWidget == nullptr)
	{
		HealthBarWidget = CreateWidget<UCWorldWidget>(GetWorld(), HealthBarWidgetClass);
		if (HealthBarWidget)
		{
			HealthBarWidget->AttachToActor = this;
			HealthBarWidget->AddToViewport();
		}
	}

}

void ACEnemyCharacter::OnHealthChanged(AActor* InstigatorActor, UCAbilitySystemComponent* OwningComp, float NewHealth, float Delta)
{
	if (Delta < 0.f)
	{

	}

}


//void ACEnemyCharacter::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
//{
//	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
//}