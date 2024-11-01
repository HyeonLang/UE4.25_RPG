#include "CEnemyCharacterAttributeSet.h"
#include "Net/UnrealNetwork.h"


UCEnemyCharacterAttributeSet::UCEnemyCharacterAttributeSet()
{

}


void UCEnemyCharacterAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCEnemyCharacterAttributeSet, Health, OldHealth);
}

void UCEnemyCharacterAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCEnemyCharacterAttributeSet, MaxHealth, OldMaxHealth);
}



void UCEnemyCharacterAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UCEnemyCharacterAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCEnemyCharacterAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);

}
