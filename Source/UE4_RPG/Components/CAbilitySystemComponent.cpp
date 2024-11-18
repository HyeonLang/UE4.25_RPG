// Fill out your copyright notice in the Description page of Project Settings.


#include "CAbilitySystemComponent.h"
#include "Attributes/CCharacterAttributeSet.h"
#include "Attributes/CEnemyCharacterAttributeSet.h"
#include "Attributes/CPlayerCharacterAttributeSet.h"

UCAbilitySystemComponent::UCAbilitySystemComponent()
{
	SetIsReplicatedByDefault(true);
	SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
}

UCAbilitySystemComponent* UCAbilitySystemComponent::GetAbilitySystemComponent(AActor* FromActor)
{
	if (FromActor) return Cast<UCAbilitySystemComponent>(FromActor->GetComponentByClass(UCAbilitySystemComponent::StaticClass()));

	return nullptr;
}

bool UCAbilitySystemComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	if (!GetOwner()->CanBeDamaged() && Delta < 0.f) // 데미지를 받지 않는 경우 추가
	{
		return false;
	}

	if (!AttributeSet)
	{
		return false;
	}

	float PrevHealth = AttributeSet->GetHealth();
	float MaxHealth = AttributeSet->GetMaxHealth();
	float NewHealth = FMath::Clamp(PrevHealth + Delta, 0.f, MaxHealth);

	float ActualDelta = NewHealth - PrevHealth;

	if (GetOwner()->HasAuthority())
	{
		AttributeSet->SetHealth(NewHealth);

		if (!FMath::IsNearlyZero(ActualDelta))
		{
			NetMulticastHealthChanged(InstigatorActor, AttributeSet->GetHealth(), ActualDelta);
		}

		/*if (ActualDela < 0.f && Health <= 0.f)
		{
			ACGameMode* GM = GetWorld()->GetAuthGameMode<ACGameMode>();
			if (GM)
			{
				GM->OnActorKilled(GetOwner(), InstigatorActor);
			}
		}*/
	}

	return !FMath::IsNearlyZero(ActualDelta);
}

void UCAbilitySystemComponent::NetMulticastHealthChanged_Implementation(AActor* InstigatorActor, float NewHealth, float Delta)
{
	OnHealthChanged.Broadcast(InstigatorActor, this, NewHealth, Delta);
}

bool UCAbilitySystemComponent::IsAlive() const
{
	return AttributeSet->GetHealth() > 0.f;
}

bool UCAbilitySystemComponent::IsFullHealth() const
{
	return FMath::IsNearlyEqual(AttributeSet->GetHealth(), AttributeSet->GetMaxHealth());
}

