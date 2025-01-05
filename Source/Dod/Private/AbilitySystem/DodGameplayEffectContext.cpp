#include "AbilitySystem/DodGameplayEffectContext.h"

FDodGameplayEffectContext* FDodGameplayEffectContext::ExtractEffectContext(struct FGameplayEffectContextHandle Handle)
{
	FGameplayEffectContext* BaseEffectContext = Handle.Get();
	if ((BaseEffectContext != nullptr) && BaseEffectContext->GetScriptStruct()->IsChildOf(
		FDodGameplayEffectContext::StaticStruct()))
	{
		return (FDodGameplayEffectContext*)BaseEffectContext;
	}

	return nullptr;
}

void FDodGameplayEffectContext::SetAbilitySource(const IDodAbilitySourceInterface* InObject, float InSourceLevel)
{
	AbilitySourceObject = MakeWeakObjectPtr(Cast<const UObject>(InObject));
}
