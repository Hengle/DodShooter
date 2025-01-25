#include "AbilitySystem/DodAbilitySystemGlobals.h"

#include "AbilitySystem/DodGameplayEffectContext.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(DodAbilitySystemGlobals)

UDodAbilitySystemGlobals::UDodAbilitySystemGlobals(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

FGameplayEffectContext* UDodAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FDodGameplayEffectContext();
}
