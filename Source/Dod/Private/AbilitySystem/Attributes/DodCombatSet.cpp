#include "AbilitySystem/Attributes/DodCombatSet.h"

#include "Net/UnrealNetwork.h"

void UDodCombatSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION_NOTIFY(UDodCombatSet, BaseDamage, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDodCombatSet, BaseHeal, COND_OwnerOnly, REPNOTIFY_Always);
}

void UDodCombatSet::OnRep_BaseDamage(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDodCombatSet, BaseDamage, OldValue);
}

void UDodCombatSet::OnRep_BaseHeal(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDodCombatSet, BaseHeal, OldValue);
}
